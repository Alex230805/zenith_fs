#ifndef FS_H
#define FS_H


#include "./fs_table.h"
#include "./fs_dir.h"
#include "./fs_file.h"


typedef struct {
    node (*getFromDevice)(uint32_t*);
    bool (*writeIntoDevice)(node);
    fs_tab (*getFsTab)(void);
    bool (*writeFsTab)(fs_tab*);
    fs_tab (*initFs)(char*, char*, int);
    bool (*mkDir)(fs_tab*,char*, char*);
    bool (*rmDir)(fs_tab*, char*, char*);
    bool (*move)(fs_tab*, char*, char*, char*);
    void (*ls)(fs_tab*, char*);
    void (*getInfo)(fs_tab*);
}Zenith_cls;

static Zenith_cls Zth = {
    get_from_device,
    write_into_device,
    get_fs_tab,
    write_fs_tab,
    init_fs,
    fs_mkdir,
    fs_rmdir,
    fs_mv,
    fs_get_dir_content,
    fs_get_info
};

#ifndef ZENITH_C
#define ZENITH_C
#endif

#endif
