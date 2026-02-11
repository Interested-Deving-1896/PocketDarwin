#!/usr/bin/env ruby
# frozen_string_literal: true

require "optparse"
require "pathname"

options = {
  xnu_dir: Pathname.new(__dir__).join("xnu-arm"),
  patch_dir: Pathname.new(__dir__).join("patches", "xnu-arm-armpba8"),
  series: "series",
  strip: "1"
}

OptionParser.new do |opts|
  opts.banner = "Usage: xnu-jit-patcher.rb [options]"
  opts.on("--xnu-dir DIR", "Path to XNU source root") { |v| options[:xnu_dir] = Pathname.new(v) }
  opts.on("--patch-dir DIR", "Path containing patch files and series") { |v| options[:patch_dir] = Pathname.new(v) }
  opts.on("--series FILE", "Series filename (default: series)") { |v| options[:series] = v }
  opts.on("-pN", "--strip N", "Patch strip level for -pN (default: 1)") { |v| options[:strip] = v.to_s }
end.parse!

xnu_dir = options[:xnu_dir].expand_path
patch_dir = options[:patch_dir].expand_path
series_file = patch_dir.join(options[:series])

abort("error: missing xnu source dir: #{xnu_dir}") unless xnu_dir.directory?
abort("error: missing patch dir: #{patch_dir}") unless patch_dir.directory?
abort("error: missing patch series file: #{series_file}") unless series_file.file?

def run_patch(xnu_dir, strip, patch_file, dry_run:)
  cmd = ["patch", "-p#{strip}", "-N", "--forward"]
  cmd << "--dry-run" if dry_run
  system(*cmd, chdir: xnu_dir.to_s, in: patch_file.to_s, out: File::NULL, err: File::NULL)
end

series_file.read.each_line do |line|
  patch_name = line.strip
  next if patch_name.empty? || patch_name.start_with?("#")

  patch_path = patch_dir.join(patch_name)
  abort("error: missing patch: #{patch_path}") unless patch_path.file?

  next unless run_patch(xnu_dir, options[:strip], patch_path, dry_run: true)

  abort("error: failed applying patch: #{patch_path}") unless run_patch(xnu_dir, options[:strip], patch_path, dry_run: false)
  puts "applied: #{patch_name}"
end
