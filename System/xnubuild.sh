#!/bin/bash
# xnubuild.sh - PocketDarwin XNU build with automatic patching

# Set PocketDarwin environment
export POCKETDARWIN=1

# Go to XNU source root
XNU_ROOT="$PWD"
cd "$XNU_ROOT"

# Apply the IORPCMessageFromMach stub patch automatically
PATCH_FILE="$XNU_ROOT/patch_IORPCMessageFromMach.patch"

if [ ! -f "$PATCH_FILE" ]; then
    cat <<'EOF' > "$PATCH_FILE"
diff --git a/DriverKit/DriverKitRPCStubs.h b/DriverKit/DriverKitRPCStubs.h
new file mode 100644
index 0000000..6666666
--- /dev/null
+++ b/DriverKit/DriverKitRPCStubs.h
@@
+#pragma once
+
+#ifdef POCKETDARWIN
+
+typedef struct IORPCMessage IORPCMessage;
+
+static inline IORPCMessage* IORPCMessageFromMach(void* msg, bool b) {
+    (void)msg;
+    (void)b;
+    return NULL;
+}
+
+#define UNUSED_RET_VAR(kret) (void)(kret)
+
+#endif // POCKETDARWIN
EOF
fi

# Apply the patch using patch program (ignore if already applied)
patch -p1 --forward < "$PATCH_FILE" || echo "Patch already applied or failed, continuing..."

# Add PocketDarwin build flags
export CFLAGS="$CFLAGS -include $XNU_ROOT/DriverKit/DriverKitRPCStubs.h -Wno-unused-but-set-variable"
export CXXFLAGS="$CXXFLAGS -include $XNU_ROOT/DriverKit/DriverKitRPCStubs.h -Wno-unused-but-set-variable"

# Build XNU
make -j$(sysctl -n hw.ncpu) \
    ARCH_CONFIGS=ARM64 \
    SDKROOT=$SDKROOT \
    OBJROOT=$OBJROOT/arm64 \
    DSTROOT=$DSTROOT/arm64 \
    RC_DARWIN_KERNEL_VERSION=23.0.0
