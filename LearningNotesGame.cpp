#include <windows.h>
#include <iostream>
#include <vector>
using namespace std;

int note_to_play = -1;

void CALLBACK midiCallback(HMIDIIN handle,
        UINT uMsg,
        DWORD dwInstance,
        DWORD dwParam1,
        DWORD dwParam2) {
        
    byte status_byte = 0;
    byte first_midi_byte = 0;
    byte second_midi_byte = 0;
    int note_index = -1;

    vector<string> notes = {"C", "C# / Db" , "D", "D# / Eb", "E", "F", "F# / Gb",
        "G", "G# / Ab", "A", "A# / Bb", "B"};

    switch (uMsg) {
    case MIM_DATA:
        /* https://docs.microsoft.com/en-us/windows/win32/multimedia/mim-data
        
        High word	High - order byte	Not used.
        Low - order byte	Contains a second byte of MIDI data(when needed).
        Low word	High - order byte	Contains the first byte of MIDI data(when needed).
        Low - order byte	Contains the MIDI status.

                High word              Low word
        not used    second byte first byte   status
        XXXX XXXX | XXXX XXXX | XXXX XXXX | XXXX XXXX */

        status_byte = dwParam1 & 0x000000FF;
        first_midi_byte = (dwParam1 >> 8) & 0x000000FF;
        second_midi_byte = (dwParam1 >> 16) & 0x000000FF;

        /* https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message
        
        Status, where nnnn = 0-15 (MIDI Channel Number 1-16):

        note off - 1000nnnn
        note on - 1001nnnn	
        */

        /* If event is note on, get a note index 0-11 starting from C */
        if (int(status_byte >> 4) == 9) {
            if (int(first_midi_byte) >= 0 && int(first_midi_byte) <= 11) {
                note_index = int(first_midi_byte);
            }
            else if (int(first_midi_byte) >= 12 && int(first_midi_byte) <= 23) {
                note_index = int(first_midi_byte) % 12;
            }
            else if (int(first_midi_byte) >= 24 && int(first_midi_byte) <= 35) {
                note_index = int(first_midi_byte) % 24;
            }
            else if (int(first_midi_byte) >= 36 && int(first_midi_byte) <= 47) {
                note_index = int(first_midi_byte) % 36;
            }
            else if (int(first_midi_byte) >= 48 && int(first_midi_byte) <= 59) {
                note_index = int(first_midi_byte) % 48;
            }
            else if (int(first_midi_byte) >= 60 && int(first_midi_byte) <= 71) {
                note_index = int(first_midi_byte) % 60;
            }
            else if (int(first_midi_byte) >= 72 && int(first_midi_byte) <= 83) {
                note_index = int(first_midi_byte) % 72;
            }
            else if (int(first_midi_byte) >= 84 && int(first_midi_byte) <= 95) {
                note_index = int(first_midi_byte) % 84;
            }
            else if (int(first_midi_byte) >= 96 && int(first_midi_byte) <= 107) {
                note_index = int(first_midi_byte) % 96;
            }
            else if (int(first_midi_byte) >= 108 && int(first_midi_byte) <= 119) {
                note_index = int(first_midi_byte) % 108;
            }
            else if (int(first_midi_byte) >= 120 && int(first_midi_byte) <= 127) {
                note_index = int(first_midi_byte) % 120;
            }

            // Check note
            if (note_to_play > -1 && note_index == note_to_play) {
                cout << "RIGTH!\n";
            }
            else if (note_to_play > -1 && note_index != note_to_play) {
                cout << "WRONG!\n";
            }

            // Make a next guess
            note_to_play = rand() % 11 + 1;
            cout << "Play " << notes[note_to_play] << '\n';
            
        }
        break;
    case MIM_OPEN:
        cout << "Received open message\n";
        break;
    case MIM_CLOSE:
        cout << "Received close message\n";
        break;
    default:
        cout << "i don't care\n";        
    }
    
    // Make a starting guess;
    if (note_to_play == -1) {
        note_to_play = rand() % 11 + 1;
        cout << "Play " << notes[note_to_play] << '\n';
    }

}


int main() {
    unsigned int devCount = midiInGetNumDevs();
    cout << devCount << " MIDI devices connected:" << endl;
    MIDIINCAPS inputCapabilities;
    for (unsigned int i = 0; i < devCount; i++) {
        midiInGetDevCaps(i, &inputCapabilities, sizeof(inputCapabilities));
        cout << "[" << i << "] " << inputCapabilities.szPname << endl;
    }

    HMIDIIN handle;

    if (midiInOpen(&handle, 0, (DWORD)midiCallback, 0, CALLBACK_FUNCTION) == MMSYSERR_NOERROR) {
        cout << "Opened successfully\n";
    }

    midiInStart(handle);
    while (true) {};

}

