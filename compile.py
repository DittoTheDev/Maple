#!/usr/bin/env python3
import os
import sys
import subprocess
import shutil

def run_command(cmd, cwd=None):
    print("Running:", cmd)
    result = subprocess.run(cmd, shell=True, cwd=cwd)
    if result.returncode != 0:
        print("Error: Command failed:", cmd)
        sys.exit(result.returncode)

def compile_maple_project():
    # Set the project root (adjust if needed)
    project_root = os.path.abspath("Maple")
    print("Project root:", project_root)

    # Compile the Maple Virtual Machine (mvm)
    mvm_cmd = "gcc main.c engine/*.c -o mvm"
    run_command(mvm_cmd, cwd=project_root)
    
    # Compile the Maple Compiler (maplec)
    maplec_cmd = "gcc compiler/maplec.c -o maplec"
    run_command(maplec_cmd, cwd=project_root)
    
    print("Compilation finished successfully!")
    print("Executables created:")
    print("  - mvm (Maple Virtual Machine)")
    print("  - maplec (Maple Compiler)")

def install_executables():
    # Define installation target directory (e.g., /usr/bin)
    install_dir = "/usr/bin"
    project_root = os.path.abspath("Maple")
    
    # Paths to the built executables
    mvm_src = os.path.join(project_root, "mvm")
    maplec_src = os.path.join(project_root, "maplec")
    
    # Target names: we can choose to name the compiler 'maple'
    mvm_dest = os.path.join(install_dir, "mvm")
    maple_dest = os.path.join(install_dir, "maple")
    
    print("Installing executables to", install_dir)
    try:
        shutil.copy(mvm_src, mvm_dest)
        os.chmod(mvm_dest, 0o755)
        print("Installed mvm to", mvm_dest)
    except Exception as e:
        print("Error installing mvm:", e)
        sys.exit(1)
    
    try:
        shutil.copy(maplec_src, maple_dest)
        os.chmod(maple_dest, 0o755)
        print("Installed maple to", maple_dest)
    except Exception as e:
        print("Error installing maple:", e)
        sys.exit(1)

def main():
    install_flag = False
    args = sys.argv[1:]
    # Check for --install flag in the arguments
    if "--install" in args:
        install_flag = True
        args.remove("--install")
    if len(args) > 0:
        # If additional arguments are passed, you could add further processing
        pass

    compile_maple_project()

    if install_flag:
        # Check if we have root privileges on Linux
        if os.name != "nt" and os.geteuid() != 0:
            print("Installation requires root privileges. Please run with sudo.")
            sys.exit(1)
        install_executables()

if __name__ == '__main__':
    main()
