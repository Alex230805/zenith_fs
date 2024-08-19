#include <stdio.h>
#include <stdlib.h>

#include "include/fs.h"

int main(void){
    fs_tab root = Zth.initFs("dev/sdc_virtual", "Zenith 1.0", MEDIUM_SIZE);
    Zth.mkDir(&root,"/", "usr");
    Zth.mkDir(&root,"/", "share");
    Zth.mkDir(&root,"/", "bin");
    Zth.mkDir(&root,"/", "lib");
    Zth.mkDir(&root,"/", "etc");
    Zth.mkDir(&root,"/", "usr");
    Zth.mkDir(&root,"/", "home");
    Zth.mkDir(&root,"/home/", "am");
    Zth.mkDir(&root,"/home/am/", "Desktop");
    Zth.mkDir(&root,"/home/am/", "Download");
    Zth.mkDir(&root,"/home/am/", "Document");
    Zth.mkDir(&root,"/home/am/", "Images");
    Zth.mkDir(&root,"/home/am/", "Music");
    Zth.ls(&root, "/");
    Zth.ls(&root, "/home");
    Zth.ls(&root, "/home/am");
    Zth.rmDir(&root, "/home/am", "Desktop");
    Zth.ls(&root, "/home/am");
    free(virtual_path);
    return 0;
}
