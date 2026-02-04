import os

def patch_file(filename):
    with open(filename, 'r') as f:
        content = f.read()
    
    # Replace os.getlogin() with 'root'
    content = content.replace('os.getlogin()', "'root'")
    
    # Also handle possible KeyError: 'USER' by providing a fallback
    content = content.replace("os.environ['USER']", "os.environ.get('USER', 'root')")
    
    with open(filename, 'w') as f:
        f.write(content)

if __name__ == '__main__':
    patch_file('/project/boinc_source/tools/make_project')
    print('Patched successfully')
