/*
 main.pp -- application's main file

 Copyright (c) 2019 Lucas V. Araujo <lucas.vieira.ar@protonmail.com>

 	This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
	 
 	This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 GNU General Public License for more details.

 	You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <iostream>
#include <string>

#include "crypt.hpp"

using namespace std;

int
main(int argc, char *argv[])
{
    /* TODO: comment the code */
    
    string text;
    string keyf;
    cout << "Enter some text: ";
    getline(cin, text);
    cout << "Export key as: ";
    getline(cin, keyf);
    cout << "You typed: " << text << endl;
    string enc = encrypt(text, keyf);
    string dec = decrypt(enc, keyf);
    cout << "Encrypted: " << enc << endl;
    cout << "Decrypted: " << dec << endl;
    return 0;
}
