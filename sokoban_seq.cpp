#include "sokoban_seq.h"

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        cout << "Usage: ./sokoban_seq input_file" << endl;
        return 0;
    }

    ifstream in_file;
    string in_line;
    char dummy;
    vector<string> input;
    int num_rows;
    

    in_file.open(argv[1]);
    //Read the number of rows and the linefeed character
    in_file >> num_rows;
    getline(in_file, in_line);

    //Read the rest of the board
    for(int i = 0; i < num_rows; ++ i)
    {
        getline(in_file, in_line);
        input.push_back(in_line);
    }

    board b(input);
/* Print the information built into the board
    b.print();

    cout << "Boxes at: " << endl;
    vector<position>::iterator ps;
    for(ps = b.boxes.begin(); ps != b.boxes.end(); ++ ps)
    {
        cout << "\t";
        (*ps).print();
    }

    cout << "Empty goals at: " << endl;
    for(ps = b.empty_goals.begin(); ps != b.empty_goals.end(); ++ ps)
    {
        cout << "\t";
        (*ps).print();
    }*/

    b.print();
    cout << b.find_path(b.player, *(new position(7, 5))) << endl;
}

void call_print(position p)
{
    cout << "\t";
    p.print();
}
