from distutils.core import setup, Extension
import os
# or
# from distutils.core import setup
def getFiles(path,suffix):
    filelist = []
    for home, dirs, files in os.walk(path):
        for filename in files:
            if filename.endswith(suffix):
                filelist.append(os.path.join(home, filename))
    return filelist
dirname, filename = os.path.split(os.path.abspath( __file__)) 
here =  dirname 
source_files = getFiles(os.path.join(here,'src'),'cpp')
setup(
    version='1.0',   
    ext_modules=[
        Extension('torch._C',
                 source_files,
                 extra_compile_args=['-std=c++17'],
                  include_dirs=[os.path.join(here,'include')]
                  )
        
    ]
)