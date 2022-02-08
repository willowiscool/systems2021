struct pop_entry {
	int year;
	int population;
	char boro[15];
};
int readCSV();
struct pop_entry *readData(int *count);
int writeData(struct pop_entry *data, int size);
void displayData(int count, struct pop_entry data[]);

struct pop_entry getInput();
int addData();
int updateData();
