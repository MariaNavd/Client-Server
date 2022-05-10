import os
import sys
import shutil

def main():
    if 'linux' not in sys.platform:
        print("Unsupported target system")
        return
    work_dir = os.getcwd()
    work_folder = os.path.join(work_dir, "build")
    if os.path.exists(work_folder):
        shutil.rmtree(work_folder)
    os.makedirs(work_folder)
    os.chdir(work_folder)
            
    os.system('cmake "%s"' % (work_dir))
    os.system('cmake --build .')

if __name__== "__main__":
    main()
