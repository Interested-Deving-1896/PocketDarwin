#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>

static const char *default_config_path = "/sdcard/OCMobileConfig.json";

static void usage(const char *prog) {
    fprintf(stderr,
        "Usage: %s [--kernel <path>] [--config <path>] [--initrd <path>] [--cmdline <string>] [--exec] [--dry-run]\n"
        "\n"
        "Prepares a Linux system and loads a Darwin kernel via kexec.\n"
        "\n"
        "Options:\n"
        "  --kernel <path>   Path to Darwin kernel image (optional if --config is used)\n"
        "  --config <path>   Path to OCMobileConfig.json (default: /sdcard/OCMobileConfig.json)\n"
        "  --initrd <path>   Optional initrd/ramdisk image\n"
        "  --cmdline <str>   Kernel command line string\n"
        "  --exec            Execute kexec immediately after loading\n"
        "  --dry-run         Print actions without executing kexec\n"
        "  -h, --help        Show this help\n",
        prog);
}

static bool file_exists(const char *path) {
    struct stat st;
    return path && stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

static int write_sysctl(const char *path, const char *value) {
    FILE *f = fopen(path, "w");
    if (!f) {
        return -1;
    }
    if (fprintf(f, "%s", value) < 0) {
        fclose(f);
        return -1;
    }
    fclose(f);
    return 0;
}

static char *read_file(const char *path, size_t *out_len) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        return NULL;
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }
    long len = ftell(f);
    if (len < 0) {
        fclose(f);
        return NULL;
    }
    if (fseek(f, 0, SEEK_SET) != 0) {
        fclose(f);
        return NULL;
    }
    char *buf = (char *)malloc((size_t)len + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }
    size_t read_len = fread(buf, 1, (size_t)len, f);
    fclose(f);
    buf[read_len] = '\0';
    if (out_len) {
        *out_len = read_len;
    }
    return buf;
}

static char *extract_json_string_value(const char *json, const char *key) {
    if (!json || !key) {
        return NULL;
    }
    size_t key_len = strlen(key);
    const char *p = json;
    while ((p = strstr(p, key)) != NULL) {
        // ensure we're at a quoted key: "key"
        if (p > json && p[-1] == '"' && p[key_len] == '"') {
            const char *colon = strchr(p + key_len + 1, ':');
            if (!colon) {
                return NULL;
            }
            const char *val = colon + 1;
            while (*val == ' ' || *val == '\t' || *val == '\n' || *val == '\r') {
                val++;
            }
            if (*val != '"') {
                return NULL;
            }
            val++;
            const char *end = strchr(val, '"');
            if (!end) {
                return NULL;
            }
            size_t len = (size_t)(end - val);
            char *out = (char *)malloc(len + 1);
            if (!out) {
                return NULL;
            }
            memcpy(out, val, len);
            out[len] = '\0';
            return out;
        }
        p += key_len;
    }
    return NULL;
}

static char *load_kernel_path_from_config(const char *config_path) {
    size_t len = 0;
    char *json = read_file(config_path, &len);
    if (!json) {
        return NULL;
    }
    const char *keys[] = {
        "darwinKernelPath",
        "darwin_kernel_path",
        "darwinKernel",
        "darwin_kernel",
        NULL
    };
    char *value = NULL;
    for (int i = 0; keys[i]; ++i) {
        value = extract_json_string_value(json, keys[i]);
        if (value) {
            break;
        }
    }
    free(json);
    return value;
}

static bool is_linux(void) {
    struct utsname uts;
    if (uname(&uts) != 0) {
        return false;
    }
    return strcmp(uts.sysname, "Linux") == 0;
}

static const char *find_kexec(void) {
    const char *candidates[] = {
        "/sbin/kexec",
        "/usr/sbin/kexec",
        "/bin/kexec",
        "/usr/bin/kexec",
        NULL
    };
    for (int i = 0; candidates[i]; ++i) {
        if (access(candidates[i], X_OK) == 0) {
            return candidates[i];
        }
    }
    return "kexec";
}

int main(int argc, char **argv) {
    const char *kernel = NULL;
    const char *config_path = default_config_path;
    const char *initrd = NULL;
    const char *cmdline = NULL;
    bool do_exec = false;
    bool dry_run = false;
    char *kernel_from_config = NULL;

    if (argc == 1) {
        usage(argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--kernel") == 0 && i + 1 < argc) {
            kernel = argv[++i];
        } else if (strcmp(argv[i], "--config") == 0 && i + 1 < argc) {
            config_path = argv[++i];
        } else if (strcmp(argv[i], "--initrd") == 0 && i + 1 < argc) {
            initrd = argv[++i];
        } else if (strcmp(argv[i], "--cmdline") == 0 && i + 1 < argc) {
            cmdline = argv[++i];
        } else if (strcmp(argv[i], "--exec") == 0) {
            do_exec = true;
        } else if (strcmp(argv[i], "--dry-run") == 0) {
            dry_run = true;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        } else {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            usage(argv[0]);
            return 1;
        }
    }

    if (!kernel) {
        kernel_from_config = load_kernel_path_from_config(config_path);
        if (!kernel_from_config) {
            fprintf(stderr, "No --kernel provided and failed to read %s.\n", config_path);
            return 1;
        }
        kernel = kernel_from_config;
    }

    if (!is_linux()) {
        fprintf(stderr, "This program must run on Linux.\n");
        return 1;
    }

    if (geteuid() != 0) {
        fprintf(stderr, "Please run as root.\n");
        return 1;
    }

    if (!file_exists(kernel)) {
        fprintf(stderr, "Kernel image not found: %s\n", kernel);
        free(kernel_from_config);
        return 1;
    }

    if (initrd && !file_exists(initrd)) {
        fprintf(stderr, "Initrd image not found: %s\n", initrd);
        free(kernel_from_config);
        return 1;
    }

    const char *kexec_disable = "/proc/sys/kernel/kexec_load_disabled";
    if (access(kexec_disable, F_OK) == 0) {
        if (!dry_run && write_sysctl(kexec_disable, "0") != 0) {
            fprintf(stderr, "Failed to enable kexec load (%s): %s\n", kexec_disable, strerror(errno));
            free(kernel_from_config);
            return 1;
        }
    }

    const char *kexec_path = find_kexec();
    if (strcmp(kexec_path, "kexec") == 0) {
        fprintf(stderr, "Warning: kexec not found in common locations; relying on PATH.\n");
    }

    printf("Preparing to load Darwin kernel via kexec...\n");
    printf("  kernel: %s\n", kernel);
    if (initrd) {
        printf("  initrd: %s\n", initrd);
    }
    if (cmdline) {
        printf("  cmdline: %s\n", cmdline);
    }
    printf("  exec after load: %s\n", do_exec ? "yes" : "no");

    if (dry_run) {
        printf("Dry run: not invoking kexec.\n");
        free(kernel_from_config);
        return 0;
    }

    // Build kexec arguments
    char *args[10];
    int idx = 0;
    args[idx++] = (char *)kexec_path;
    args[idx++] = (char *)"-l";
    args[idx++] = (char *)kernel;
    if (initrd) {
        args[idx++] = (char *)"--initrd";
        args[idx++] = (char *)initrd;
    }
    if (cmdline) {
        args[idx++] = (char *)"--command-line";
        args[idx++] = (char *)cmdline;
    }
    args[idx] = NULL;

    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed: %s\n", strerror(errno));
        free(kernel_from_config);
        return 1;
    }
    if (pid == 0) {
        execvp(kexec_path, args);
        fprintf(stderr, "Failed to exec kexec: %s\n", strerror(errno));
        _exit(127);
    }

    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        fprintf(stderr, "waitpid failed: %s\n", strerror(errno));
        free(kernel_from_config);
        return 1;
    }
    if (status != 0) {
        fprintf(stderr, "kexec load failed (status %d).\n", status);
        free(kernel_from_config);
        return 1;
    }

    if (do_exec) {
        printf("Executing kexec...\n");
        char *exec_args[] = { (char *)kexec_path, (char *)"-e", NULL };
        execvp(kexec_path, exec_args);
        fprintf(stderr, "Failed to exec kexec -e: %s\n", strerror(errno));
        free(kernel_from_config);
        return 1;
    }

    free(kernel_from_config);
    printf("kexec load complete.\n");
    return 0;
}
