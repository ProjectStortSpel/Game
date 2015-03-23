libatc
======

[![Build Status](https://travis-ci.org/h2so5/libatc.png?branch=master)](https://travis-ci.org/h2so5/libatc)

[AttacheCase](http://github.com/hibara/AttacheCase) Encryption/Decryption Library for C++ and C#/.NET

* Decryption for *.atc and *.exe format
* Encryption for *.atc format

libatc doesn't provide any platform-dependent functions.
   

## Usage
======

### Build

#### Windows (C++ and C#)
Open __libatc/msvc/libatc.sln__

#### Mac OS X
Open __libatc/xcode/libatc.xcodeproj__

#### Linux (GCC)

You need zlib header and binary.

```bash
$ cd libatc
$ make
$ make test
```

### Decryption

```cpp
#include <iostream>
#include <fstream>

// Using boost for directory operations
#include <boost/filesystem.hpp>

#include "ATCUnlocker.h"

using namespace std;
using namespace boost::filesystem;

int main()
{
    char key[ATC_KEY_SIZE] = "This is a pen.";
    char atc_filename[] = "test.atc";

    ATCUnlocker unlocker;
    ifstream ifs(atc_filename, ifstream::binary);

    ATCResult result = unlocker.open(&ifs, key);
    if (result == ATC_OK) {
    
        // Get the number of files and directories
        int length = unlocker.getEntryLength();

        for (int i = 0; i < length; ++i) {
        
            // Get entry inforamation
            ATCFileEntry entry;
            unlocker.getEntry(&entry, i);

            // Is it a directory?
            if (entry.size == -1) {
            
                // Create a directory
                if (!exists(entry.name_sjis)) {
                    create_directory(entry.name_sjis);
                }
                
            } else {
            
                // Write extracted data
                ofstream ofs(entry.name_sjis, ios::binary);
                unlocker.extractFileData(&ofs, &ifs, entry.size);
                
            }
        }
    } else if (result == ATC_ERR_WRONG_KEY) {
        cout << "Error: Wrong password." << endl;
    } else {
        cout << "Error: Cannot open." << endl;
    }
    
    unlocker.close();
    
    return 0;
}
```



### Encryption

```cpp
#include <iostream>
#include <fstream>
#include <ctime>
#include <zlib.h>

#include "ATCLocker.h"

using namespace std;

int main()
{
    char key[ATC_KEY_SIZE] = "This is a pen.";
    char atc_filename[] = "test.atc";

    ATCLocker locker;

    // Set the options before open()
    locker.set_compression_level(Z_BEST_COMPRESSION);
    locker.set_passwd_try_limit(10);
    locker.set_self_destruction(false);
    locker.set_create_time(time(NULL));

    ofstream ofs(atc_filename, ifstream::binary);
    locker.open(&ofs, key);

    // Add "dir/"
    {
        ATCFileEntry entry;
            entry.attribute = 16;
            entry.size = -1;
            entry.name_sjis = "dir\\";
            entry.name_utf8 = "dir\\";
            entry.change_unix_time = time(NULL);
            entry.create_unix_time = time(NULL);

        locker.addFileEntry(entry);
    }

    // Prepare input files
    ifstream ifs_foo_txt("foo.txt", ifstream::binary);
        ifs_foo_txt.seekg(0, ios::end);
        size_t foo_txt_size = ifs_foo_txt.tellg();
        ifs_foo_txt.seekg(0, ios::beg);

    ifstream ifs_bar_txt("bar.txt", ifstream::binary);
        ifs_bar_txt.seekg(0, ios::end);
        size_t bar_txt_size = ifs_bar_txt.tellg();
        ifs_bar_txt.seekg(0, ios::beg);


    // Add "dir/foo.txt"
    {
        ATCFileEntry entry;
            entry.attribute = 0;
            entry.size = foo_txt_size;
            entry.name_sjis = "dir\\foo.txt";
            entry.name_utf8 = "dir\\foo.txt";
            entry.change_unix_time = time(NULL);
            entry.create_unix_time = time(NULL);

        locker.addFileEntry(entry);
    }

    // Add "dir/bar.txt"
    {
        ATCFileEntry entry;
            entry.attribute = 0;
            entry.size = bar_txt_size;
            entry.name_sjis = "dir\\bar.txt";
            entry.name_utf8 = "dir\\bar.txt";
            entry.change_unix_time = time(NULL);
            entry.create_unix_time = time(NULL);

        locker.addFileEntry(entry);
    }

    // Write the header
    locker.writeEncryptedHeader(&ofs);

    // Write the file data in the same order as addFileEntry()
    locker.writeFileData(&ofs, &ifs_foo_txt, foo_txt_size);
    locker.writeFileData(&ofs, &ifs_bar_txt, bar_txt_size);
    
    locker.close();
    
    return 0;
}
```



## Copyright notice: libatc
Copyright (c) 2013 h2so5 <mail@h2so5.net>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.



## Copyright notice: AttacheCase

The BSD 3-Clause License

????????(AttacheCase) Copyright (c) 2002-2013, Mitsuhiro Hibara ( http://hibara.org ) All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

·Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. ·Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. ·Neither the name of the "HIBARA.ORG" nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.