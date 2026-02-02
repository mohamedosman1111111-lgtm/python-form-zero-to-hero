#include <iostream>
#include <vector>
#include <fstream>
#include<conio.h>
#include<string>
#include<Windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;
struct AduioPlayer
{
	string SoraName;
	string SoraType;
	string SoraPath;
	AduioPlayer(string name, string type, string path) {
		SoraName = name;
		SoraType = type;
		SoraPath = path;
	}

};
struct Node
{
	AduioPlayer* data;
	Node* next;
	Node* prev;
	Node(AduioPlayer* Soraname) { //conect node to sora
		data = Soraname;
		next = nullptr;
		prev = nullptr;
	}
};

class PlayList
{
private:
	string name;
	Node* head;
	Node* tail;
	size_t size;


public:
	PlayList(string Playlistname) {
		name = Playlistname;
		head = nullptr;
		tail = nullptr;
		size = 0;
	}

	void clear() { //h
		while (head) {
			Node* temp = head;
			head = head->next;
			delete temp->data;
			delete temp;
		}
		head = nullptr;
		tail = nullptr;
		size = 0;
	}

	~PlayList() {
		clear();
	}

	void AddSoraAtEnd(string soraname, string type, string sorapath) {
		AduioPlayer* newsora = new AduioPlayer(soraname, type, sorapath);
		Node* newnode = new Node(newsora);
		if (!head) {
			head = tail = newnode;

		}
		else {
			tail->next = newnode;
			newnode->prev = tail;
			tail = newnode;

		}
		size++;

	}

	void DisplyAlSor() {
		if (!head) {
			cout << "PlayList: " << name << " IsEmpty\n";
			return;
		}
		else {
			Node* temp = head;
			int i = 1;
			while (temp) {
				cout << i << "." << "SoraName: " << temp->data->SoraName << "       SoraType: " << temp->data->SoraType << "       SoraPath: " << temp->data->SoraPath;
				temp = temp->next;
				i++;
				cout << endl;
			}

		}
	}
	string getname()const { //h
		return name;
	}
	void playMP3(const string& filePath) {

		mciSendStringA("close all", NULL, 0, NULL);

		// Open and play WITHOUT wait (asynchronous)
		string openCmd = "open \"" + filePath + "\" type mpegvideo alias current";
		mciSendStringA(openCmd.c_str(), NULL, 0, NULL); //open
		mciSendStringA("play current", NULL, 0, NULL); // play
	}
	Node* getHead() { //h
		return head;
	}
	bool RemoveSurahFromPlayList(string& SurahName) {
		Node* temp = head;
		while (temp) {
			if (temp->data->SoraName == SurahName) {
				if (temp == head) {
					head = temp->next;
				}
				if (temp == tail) {
					tail = temp->prev;
				}
				if (temp->next) { //there are node after temp or temp->next!=nullptr
					temp->next->prev = temp->prev;
				}if (temp->prev) {
					temp->prev->next = temp->next;
				}
				delete temp->data;
				delete temp;
				if (size > 0)
				{
					size--;
				}
				return true;
			}
			temp = temp->next;
		}
		return false;

	}

	string getSurahNameAtIndex(int index) { //h
		if (index < 0 || index >= size) {
			return "";
		}
		Node* temp = head;
		for (int i = 0; i < index; i++) {
			temp = temp->next;
		}
		return temp->data->SoraName;

	}
	AduioPlayer* ExtractSurahAtIndex(int index) { //h
		if (index < 0 || index >= size) {
			return nullptr;
		}

		Node* temp = head;
		for (int i = 0; i < index; i++) {
			temp = temp->next;
		}
		if (temp == head) {
			head = temp->next;
		}
		if (temp == tail) {
			tail = temp->prev;
		}
		if (temp->next) {   //ال بعد النود ب ال قبل ال نود
			temp->next->prev = temp->prev;
		}
		if (temp->prev) {
			temp->prev->next = temp->next; //هربط ال قبل النود بال ال بعد النود
		}

		AduioPlayer* data = temp->data;
		delete temp; //temp only not temp->data
		size--;
		return data;
	}

	void InsertSurahAtTarget(int index, AduioPlayer* track) {//h

		if (index <= 0) {

			Node* newnode = new Node(track);
			if (!head) { //Empty
				head = tail = newnode;
			}
			else  //first one
			{
				newnode->next = head;
				head->prev = newnode;
				head = newnode;
			}
			size++;

		}
		else if (index >= size) { //at end
			AddSoraAtEnd(track->SoraName, track->SoraType, track->SoraPath);

		}
		else //middle
		{
			Node* temp = head;

			for (int i = 0; i < index; i++) {
				temp = temp->next;
			}
			Node* newnode = new Node(track);
			newnode->prev = temp->prev;
			newnode->next = temp;
			temp->prev->next = newnode;
			temp->prev = newnode;
			size++;
		}
	}

	int getsize()const { //h 
		return size;
	}









};
class PlaylistManger {
private:
	vector<PlayList*> playlists;


public:
	void createPlaylist() {  //Need to make Destuctour at end of code
		string name;
		cout << "Enter playlist name: ";
		cin >> name;

		// check if playlist exists
		for (auto p : playlists) {
			if (p->getname() == name) {
				cout << "Playlist already exists!\n";
				return;
			}
		}

		PlayList* newP = new PlayList(name);
		playlists.push_back(newP);

		cout << "Playlist created successfully.\n";
	}

	void removePlaylist() {//need edit if i write playlist not found 
		cout << "Current PlayLists Are: \n";
		DisplayAllPlaylists();
		string name;
		cout << "Enter playlist name to delete: ";
		cin >> name;

		for (int i = 0; i < playlists.size(); i++) {
			if (playlists[i]->getname() == name) {

				delete playlists[i];  // free memory
				playlists.erase(playlists.begin() + i);

				cout << "Playlist removed successfully.\n";
				return;
			}

		}
		cout << "Invaild PlayList Name\n";


	}
	void DisplayAllPlaylists() {
		if (playlists.empty()) {
			cout << "No playlists available yet.\n";
			return;
		}

		cout << "\nID  Playlists \n";
		cout << "==============\n";
		for (size_t i = 0; i < playlists.size(); i++) {
			cout << i + 1 << ". " << playlists[i]->getname() << endl;
		}
		cout << "----------------------\n";
	}

	void AddShuraToPlaylist() {
		if (playlists.empty()) {
			cout << "No PlayLists Found\n";
			return;
		}

		cout << "The Current PlayLists are : \n";

		DisplayAllPlaylists();
		char answer;
		string plname, srname, type, pathname;
		cout << "Enter PlayList Name: ";
		cin >> plname;

		PlayList* Targetplaylist = nullptr;
		for (auto p : playlists) {
			if (p->getname() == plname) {
				Targetplaylist = p;
				break;
			}

		}
		if (!Targetplaylist) {
			cout << "PlayList " << plname << " Not Found\n";
			return;
		}


		do {
			cout << "Enter Shura Name: ";
			cin >> srname;

			cout << "Enter Shura Type: ";
			cin >> type;

			cout << "Enter Shura Path: ";
			cin >> pathname;

			Targetplaylist->AddSoraAtEnd(srname, type, pathname);
			cout << "Add Other Shura To Current PlayList ? Press(y/n)";
			cin >> answer;
		} while (answer == 'y' || answer == 'Y');

	}


	void RemoveSurahFromPlayList() {
		if (playlists.empty()) {
			cout << "No PlayLists Found\n";
			return;
		}
		cout << "Current PlayLists are: \n";
		DisplayAllPlaylists();
		string plname;
		cout << "Enter PlayList Name: ";
		cin >> plname;
		PlayList* TargetPlayList = nullptr;
		for (auto p : playlists) {
			if (p->getname() == plname) {
				TargetPlayList = p;
				break;
			}
		}
		if (!TargetPlayList) {
			cout << "PlayList " << plname << " Not Found\n";
			return;
		}
		string Srname;
		cout << "Enter Surah Name: ";
		cin >> Srname;
		if (TargetPlayList->RemoveSurahFromPlayList(Srname)) {
			cout << "Surah " << Srname << " Removed From PlayList\n";
		}
		else
		{
			cout << "Surah " << Srname << " Not Found !!\n";
		}


	}

	void displayAllPlaylistsSurahs() { //need edit
		if (playlists.empty()) {
			cout << "No playlists found.\n";
			return;
		}

		for (auto p : playlists) {
			cout << "Playlist: " << p->getname() << "\n";
			p->DisplyAlSor();

			cout << "\n-----------------------------\n";
		}
	}
	void PlayShuraFromPlayList() {
		if (playlists.empty()) {
			cout << "No PlayLists Found\n";
			return;
		}

		cout << "Current PlayLists are: \n";
		DisplayAllPlaylists();
		string plname;
		cout << "Enter PlayList Name: ";
		cin >> plname;
		PlayList* TargetPlayList = nullptr;
		for (auto p : playlists) {
			if (p->getname() == plname) {
				TargetPlayList = p;
				break;
			}
		}
		if (!TargetPlayList || !TargetPlayList->getHead()) {
			cout << "Empty PlayList OR PlayList Not Found\n";
			return;
		}

		Node* Current = TargetPlayList->getHead();

		cout << "Playing: " << Current->data->SoraName << "...\n";
		TargetPlayList->playMP3(Current->data->SoraPath);


		//---------------------------------------------------------------------

		while (true) { //look again here
			if (_kbhit()) { //if user press key on keybord return true


				int key = _getch(); //read key without saw on screen
				if (key == 0 || key == 224) {
					key = _getch();
				}
				if (key == 'q' || key == 'Q') {
					mciSendStringA("close current", NULL, 0, NULL);
					cout << "You Stopped The Current PlayList.\n";
					break;
					//should be menu as i think
				}


				switch (key) {
				case 77:// right
					if (Current->next) {
						Current = Current->next;
					}
					else
					{
						cout << "End of PlayList Reached\n";
						Current = TargetPlayList->getHead();
					}
					cout << "Now Playing: " << Current->data->SoraName << "...\n";
					TargetPlayList->playMP3(Current->data->SoraPath);

					break;
					//======================================================
				case 75: //left
					if (Current->prev) {
						Current = Current->prev;
					}
					else
					{
						cout << "Start of PlayList Reached\n";

					}
					cout << "Now Playing: " << Current->data->SoraName << "...\n";
					TargetPlayList->playMP3(Current->data->SoraPath);

					break;
				case 72: //up


					mciSendStringA("pause current", NULL, 0, NULL);
					cout << "Surah Paused\n";

					break;
				case 80: //down



					mciSendStringA("resume current", NULL, 0, NULL);
					cout << "Surah Resumed\n";


					break;

				default:
					break;
				}

			}

		}
	}
	void DisplaySurahsonPlayList() {
		if (playlists.empty()) {
			cout << "No PlayList Found\n";
			return;
		}
		cout << "Current PlayLists are: \n";
		DisplayAllPlaylists();
		string plname;
		cout << "Enter PlayList Name: ";
		cin >> plname;
		cout << "=========================\n";
		PlayList* TargetPlayList = nullptr;
		for (auto p : playlists) {
			if (p->getname() == plname) {
				TargetPlayList = p;
				break;
			}
		}
		if (!TargetPlayList) {
			cout << "PlayList " << plname << " Not Found";
			return;
		}
		cout << "         PlayList: " << plname << "\n";
		cout << "======================================================\n";
		TargetPlayList->DisplyAlSor();

	}

	void run() {
		int choice;
		do {
			showMenu();
			cin >> choice;

			switch (choice) {
			case 1: createPlaylist(); break;
			case 2: AddShuraToPlaylist(); break;
			case 3: RemoveSurahFromPlayList(); break;
			case 4: UpdateSurahPostionOnPlayList(); break;
			case 5: DisplayAllPlaylists(); break;
			case 6: displayAllPlaylistsSurahs(); break;
			case 7: DisplaySurahsonPlayList(); break;
			case 8: PlayShuraFromPlayList(); break;
			case 9: SaveAllPlaylists(); break;
			case 10: LoadAllPlaylists(); break;
			case 11: removePlaylist(); break;
			case 12:SearchSurah(); break;
			case 13:ShowProjectStats(); break;
			case 14: FilterSurahsByType(); break;
			case 15: UpdateSurahonPlayList(); break;
			case 16: cout << "GoodBye\n Project By:\n                    |Mohamed Fawzy|\n" << "                    |Roaa Ahmed|\n" << "                    |Hassna Mohamed|\n" << "                    |Salma Ahmed|\n" << "                    |Rawan Mahmoud|\n" << "                    |Amir Rafat|"; break;

			default: cout << "Invalid choice. Try again.\n";
			}
		} while (choice != 16);
	}




	void SaveAllPlaylists() {
		if (playlists.size() == 0) {
			cout << "No playlists to save.\n";
			return;
		}
		for (auto p : playlists) {
			string filename = p->getname() + ".txt";
			ofstream f(filename);

			if (!f.is_open()) {
				cout << "Error saving " << p->getname() << endl;
				continue;
			}

			Node* temp = p->getHead();
			while (temp) {
				f << temp->data->SoraName << "|" << temp->data->SoraType << "|" << temp->data->SoraPath << "\n";
				temp = temp->next;
			}
			f.close();
		}

		cout << "\n All playlists saved.\n";
	}

	//---------------- LOAD ----------------//Error Here and need Edit if we try to load without saved playlist print it
	void LoadAllPlaylists() {
		WIN32_FIND_DATAA data;
		HANDLE h = FindFirstFileA("*.txt", &data);

		if (h == INVALID_HANDLE_VALUE) {
			return;
		}

		do {
			string fname = data.cFileName;
			string plName = fname.substr(0, fname.find(".txt"));

			PlayList* pl = nullptr;

			// Check if already loaded
			for (auto p : playlists)
				if (p->getname() == plName)
					pl = p;


			if (!pl) {
				pl = new PlayList(plName);
				playlists.push_back(pl);
			}

			ifstream f(fname);
			string line;

			while (getline(f, line)) {
				int p1 = line.find("|");
				int p2 = line.find("|", p1 + 1);

				pl->AddSoraAtEnd(
					line.substr(0, p1),
					line.substr(p1 + 1, p2 - p1 - 1),
					line.substr(p2 + 1)
				);
			}

			f.close();

		} while (FindNextFileA(h, &data));

		FindClose(h);

		cout << " Loaded previous playlists.\n";
	}









	void UpdateSurahPostionOnPlayList() {
		if (playlists.empty()) {
			cout << "No PlayList Found\n";
			return;
		}
		cout << "Current PlayLists are: \n";
		DisplayAllPlaylists();
		string plname;
		cout << "Enter PlayList Name: ";
		cin >> plname;
		PlayList* TargetPlayList = nullptr;
		for (auto p : playlists) {
			if (p->getname() == plname) {
				TargetPlayList = p;
				break;
			}
		}
		if (!TargetPlayList) {
			cout << "PlayList " << plname << " Not Found";
			return;
		}
		cout << "\nCurrent Surahs in playlist " << plname << ":\n";
		TargetPlayList->DisplyAlSor();
		string SurahName;
		cout << "Enter Surah Name: ";
		cin >> SurahName;

		int newPos;
		cout << "Enter New Position: ";
		cin >> newPos;


		int CurrentIndex = -1;
		Node* temp = TargetPlayList->getHead();
		int i = 0;
		while (temp) {
			if (temp->data->SoraName == SurahName) {
				CurrentIndex = i;
				break;
			}
			temp = temp->next;
			i++;
		}

		if (CurrentIndex == -1) {
			cout << " Surah '" << SurahName << "' not found in playlist.\n";
			return;
		}


		if (newPos<0 || newPos>TargetPlayList->getsize()) {
			cout << "Invalid Position !\n";
			return;
		}if (CurrentIndex == newPos)
		{
			cout << "Surah is already on this Position\n";
			return;
		}
		AduioPlayer* Surah = TargetPlayList->ExtractSurahAtIndex(CurrentIndex);
		if (!Surah) {
			cout << "Failed to Extract Surah.\n";
			return;
		}

		if (newPos > CurrentIndex)
		{
			newPos--;
		}

		TargetPlayList->InsertSurahAtTarget(newPos, Surah);
		cout << "Surah '" << SurahName << "'Moved to Postion " << newPos + 1 << ".\n";

	}
	void showMenu() {
		cout << "\n";
		cout << "                                     +-----------------------------------------------+\n";
		cout << "                                     |           Quraan Playlist Manager Menu        |\n";
		cout << "                                     +-----------------------------------------------+\n";
		cout << "                                     | 1.  Add a new playlist                        |\n";
		cout << "                                     | 2.  Add Shurah to an existing playlist        |\n";
		cout << "                                     | 3.  Remove Shurah from an existing playlist   |\n";
		cout << "                                     | 4.  Update the order of existing playlist     |\n";
		cout << "                                     | 5.  Display All Current Playlists             |\n";
		cout << "                                     | 6.  Display all Playlists Shurah              |\n";
		cout << "                                     | 7.  Display Shurah in a specific playlist     |\n";
		cout << "                                     | 8.  Play Shurah from specific playlist        |\n";
		cout << "                                     |     Use left arrow (<-) to play the previous shurah\n";
		cout << "                                     |     Use right arrow (->) to play the next shurah\n";
		cout << "                                     |     Use up arrow (^) to pause the current shurah\n";
		cout << "                                     |     Use down arrow (v) to resume the current shurah\n";
		cout << "                                     |     Press q to exit current playlist & return back to menu\n";
		cout << "                                     | 9.  Save an existing playlist to a file       |\n";
		cout << "                                     | 10. Load an existing playlist from a file     |\n";
		cout << "                                     | 11. Remove an existing playlist               |\n";
		cout << "                                     | 12. Search For AnySurah You Want              |\n";
		cout << "                                     | 13. Show Project Stats                        |\n";
		cout << "                                     | 14. Filter Surahs by Type (Makya/Madinah)     |\n";
		cout << "                                     | 15. Update Surah on PlayList                  |\n";
		cout << "                                     | 16. Exit                                      |\n";
		cout << "                                     +-----------------------------------------------+\n";
		cout << "Your choice: ";
	}
	//=================================================================================================================================================
	//------------------------------------------------------More Functions-----------------------------------------------------------------------------

	void SearchSurah() {
		if (playlists.empty()) {

			cout << "No PlayLists Found";
			return;
		}
		string srname;
		cout << "Enter Surah Name To Search: ";
		cin >> srname;
		bool Search = false;


		for (auto p : playlists) {
			Node* temp = p->getHead();
			while (temp) {
				if (temp->data->SoraName == srname) {
					cout << "Found Surah " << srname << " In PlayList: " << p->getname();
					Search = true;
				}
				temp = temp->next;

			}
		}
		if (!Search) {  //not Found
			cout << "Surah " << srname << " Not Found In Any PlayList\n";


		}



	}
	void ShowProjectStats()  //display how much playlist and how much surahs
	{
		int totalPlaylists = playlists.size();
		int totalSurahs = 0;

		for (auto p : playlists) {
			totalSurahs += p->getsize();
		}

		cout << "\n================ Project Stats ================\n";
		cout << "Total PlayLists: " << totalPlaylists << endl;
		cout << "Total Surahs  : " << totalSurahs << endl;
		cout << "================================================\n";
	}

	void FilterSurahsByType() {
		if (playlists.empty()) {
			cout << "No playlists found.\n";
			return;
		}

		string type;
		cout << "Enter type (Makya / Madinah): ";
		cin >> type;

		cout << "\nAll Surahs on type " << type << "\n";
		cout << "--------------------------------------\n";

		int counter = 1;

		for (auto p : playlists) {
			Node* temp = p->getHead();
			while (temp) {
				if (temp->data->SoraType == type) {
					cout << counter << ". " << temp->data->SoraName
						<< "   | Playlist: " << p->getname()
						<< "   | Path: " << temp->data->SoraPath << "\n";
					counter++;
				}
				temp = temp->next;
			}
		}

		if (counter == 1) cout << "No Surahs found for this type.\n";

		cout << "--------------------------------------\n";
	}


	void UpdateSurahonPlayList() {
		if (playlists.empty()) {
			cout << "No PlayList Found\n";
			return;
		}
		cout << "Current PlayLists Are: \n";
		DisplayAllPlaylists();
		string Plname;
		cout << "Enter PlayList Name: ";
		cin >> Plname;
		PlayList* TargetPlaylist = nullptr;
		for (auto p : playlists) {
			if (p->getname() == Plname) {
				TargetPlaylist = p;
				break;
			}
		}
		if (!TargetPlaylist) {
			cout << "PlayList " << Plname << " Not Found\n";
			return;
		}
		cout << "Current Surahs on PlayList Are: \n";
		TargetPlaylist->DisplyAlSor();
		string oldsrname, newsrname, type, path;

		cout << "Enter Name of Surah That You Want to Replace: ";
		cin >> oldsrname;

		Node* temp = TargetPlaylist->getHead();
		bool found = false;
		while (temp) {
			if (temp->data->SoraName == oldsrname) {
				found = true;
				break;
			}
			temp = temp->next;
		}
		if (!found) {
			cout << "Surah " << oldsrname << " Not Found\n";
			return;
		}
		cout << "Enter New Surah Name: ";
		cin >> newsrname;
		cout << "Enter New Surah Type: ";
		cin >> type;
		cout << "Enter New Surah Path: ";
		cin >> path;
		if (temp != nullptr) {


			temp->data->SoraName = newsrname;
			temp->data->SoraType = type;
			temp->data->SoraPath = path;
			cout << "Surah " << oldsrname << " Updated To " << newsrname << " In The Same Postion";
		}
		else {
			cout << "Error temp is nullptr";
		}




	}



};


int main() {

	PlaylistManger a;

	a.run();



	return 0;
}