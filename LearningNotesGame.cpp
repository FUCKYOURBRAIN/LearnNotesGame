#include <windows.h>
#include <iostream>
#include <vector>
#include <time.h>
using namespace std;

int note_to_play = -1;
int game_mode = 0;

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

    vector<string> stave = {
        "-----\n     \n-----\n     \n-----\n     \n=====\n     \n-----\n     \n -o- \n", // C4
        "-----\n     \n-----\n     \n-----\n     \n=====\n     \n-----\n  o  \n", // D4
        "-----\n     \n-----\n     \n-----\n     \n=====\n     \n--o--\n", // E4
        "-----\n     \n-----\n     \n-----\n     \n=====\n  o  \n-----\n", // F4
        "-----\n     \n-----\n     \n-----\n     \n==o==\n     \n-----\n", // G4
        "-----\n     \n-----\n     \n-----\n  o  \n=====\n     \n-----\n", // A4
        "-----\n     \n-----\n     \n--o--\n     \n=====\n     \n-----\n", // B4
        "-----\n     \n-----\n  o  \n-----\n     \n=====\n     \n-----\n", // C5
        "-----\n     \n--o--\n     \n-----\n     \n=====\n     \n-----\n", // D5
        "-----\n  o  \n-----\n     \n-----\n     \n=====\n     \n-----\n", // E5
        "--o--\n     \n-----\n     \n-----\n     \n=====\n     \n-----\n", // F5
        "  o  \n-----\n     \n-----\n     \n-----\n     \n=====\n     \n-----\n", // G5
        "--o--\n     \n-----\n     \n-----\n     \n-----\n     \n=====\n     \n-----\n", // A5
    };

    vector<int> numbers = {
        60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79, 81
    };

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
            if (game_mode == 1) {
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
                note_to_play = rand() % 12;
                cout << "Play " << notes[note_to_play] << '\n';
            }
            else if (game_mode == 2) {
                //cout << int(first_midi_byte) << '\n';
                if (numbers[note_to_play] == int(first_midi_byte)) {
                    cout << "RIGTH!\n";
                }
                else {
                    cout << "WRONG!\n";
                }

                // Make a next guess
                note_to_play = rand() % 13;
                //cout << numbers[note_to_play] << '\n';
                cout << stave[note_to_play] << '\n';
            }
            
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
        if (game_mode == 1) {
            note_to_play = rand() % 12;
            cout << "Play " << notes[note_to_play] << '\n';
        }
        else if (game_mode == 2) {
            note_to_play = rand() % 13;
            //cout << numbers[note_to_play] << '\n';
            cout << stave[note_to_play] << '\n';
        }
    }

}


int main() {
    unsigned int time_ui = unsigned int(time(NULL));
    srand(time_ui);

    cout << "Choose a game mode. Enter 1 for name mode, enter 2 for stave mode\n";
    cin >> game_mode;

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

