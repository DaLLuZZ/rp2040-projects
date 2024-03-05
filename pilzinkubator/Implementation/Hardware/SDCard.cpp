//
// Created by steffen on 19.01.24.
//

#include "SDCard.h"

bool SDCard::mount() {
    initFS();
    // Register FatFs driver for Posix-style I/O
    // (fopen, fclose, fprintf, ...)
    posix_io::inst.register_fileio( *fs );

    // Mount the SD card. Using this command,
    // the SD-card is opened by the FatFs driver
    ////////////////////////////////////////////
    mounted = (fs->mount() == FatFs::FR_OK);
    return mounted;
}

bool SDCard::unmount() {
    // Unmount the SD card
    (fs->umount() == FatFs::FR_OK);
    posix_io::inst._file_io = nullptr;

    mounted = false;
    return mounted;
}

bool SDCard::hasFile(const char* filename) {
    if(!mounted)
    {
        printf("SD Karte nicht eingehangen!");
        return false;
    }

    FatFs:: FILE f;
    FatFs::FRESULT r;
    r = fs->open(&f, filename, FA_OPEN_EXISTING);
    return (r == FatFs::FR_OK);
}

String SDCard::readFile(const char *filename) {
    if(!mounted)
    {
        printf("SD Karte nicht eingehangen!");
        return nullptr;
    }

    FatFs::FILE f;
    FatFs::FRESULT r;
    r = fs->open(&f, filename, FA_OPEN_EXISTING | FA_READ);
    if(r == FatFs::FR_NO_FILE)
    {
        printf("Datei %s konnte nicht gefunden werden!\n", filename);
        return nullptr;
    }
    else if(r != FatFs::FR_OK)
    {
        printf("Fehler beim Öffnen der Datei %s!\n", filename);
        return -1;
    }

    ulong s = f_size(&f)+1;

    char buf[s];
    uint16_t *n = nullptr;
    fs->read(&f, buf, s, n);
    buf[s-1] = '\0';

    fs->close(&f);

    return {buf};
}

bool SDCard::writeFile(const char *filename, String text, bool append) {
    if(!mounted)
    {
        printf("SD Karte nicht eingehangen!\n");
        return false;
    }

    // Open file for reading and writing
    FatFs::FILE f;
    FatFs::FRESULT r;
    if(append)
    {
        r = fs->open(&f, filename, FA_OPEN_APPEND | FA_WRITE);
    }
    else
    {
        r = fs->open(&f, filename, FA_CREATE_ALWAYS | FA_WRITE);
    }

    if(r != FatFs::FR_OK)
    {
        printf("SD Karte konnte nicht geöffnet werden!\n", stderr);
        return false;
    }

    uint16_t bw;
    fs->write(&f, text.c_str(), sizeof(char)*text.size(), &bw);

    if(bw < sizeof(char)*text.size())
    {
        fs->close(&f);
        printf("Kein Speicherplatz auf der SD-Karte!\n", stderr);
        return false;
    }

    // Finally close file
    fs->close(&f);
    return true;
}

bool SDCard::hasSDCard() {
    if(mounted)
        return true;

    mount();
    if(mounted)
    {
        unmount();
        return true;
    }
    return false;
}
