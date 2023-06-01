#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int readDimensions(FILE* input, int *m, int *n); // Διαβάζει από το αρχείο εισόδου τις διαστάσεις (M x N) του πίνακα με τα κεφαλαία γράμματα
int readCrossword(FILE* input, char** array, int m); // Διαβάζει από το αρχείο εισόδου τον πίνακα με τα κεφαλαία γράμματα.
int readNumOfHiddenWords(FILE* input, int* K); // Διαβάζει το πλήθος των κρυμμένων λέξεων του κρυπτόλεξου
int readHiddenWords(FILE* input, char** array, int K); // Διαβάζει τον πίνακα με τις κρυμμένες λέξεις
void sortHidden(char** array, int n); // Ταξινόμηση του πίνακα με τις κρυμμένες λέξεις κατά αλφαβητική σειρά
void findWords(char** array, char** hidden_words, int m, int n, int K); // Συνάρτηση οι οποία εντοπίζει τις λέξεις στον πίνακα 


int main(int argc, char **argv)
{
  // system("chcp 1253"); Για τα ελληνικα σε windows

	// Έλεγχος αν έχει δοθεί σωστός αριθμός ορισμάτων από το command line
	if(argc =! 2)
	{
		printf("Δεν δόθηκε αρχείο...\n");
		exit(1);
	}

	// Άνοιγμα αρχείου εισόδου για διάβασμα
	FILE *file = fopen(argv[1], "r");  
	
	// Αν αποτύχει το άνοιγμα του αρχείου
	if (file == NULL)
	{
		printf("Σφάλμα: Δεν δόθηκε αρχείο εισόδου...\n");
		exit(2);
	}

	int m, n; // Δήλωση μεταβλητών που θα δέχονται τις διαστάσεις του πίνακα με τα κεφαλαία γράμματα
	
	// Κλήση συνάρτησης που δέχεται τις διαστάσεις από το αρχείο και τις επιστρέφει με pointers
	if( ! readDimensions(file, &m, &n)) // Αν δεν είναι επιτυχής η ανάγνωση δεδομένων 
	{ 
		printf("Παρουσιάστηκε σφάλμα κατά την ανάγνωση των διαστάσεων του πίνακα\n");
		exit(3);
	}

	n++; // Αύξηση του n για τον χαρακτήρα \n.

	// Για το διάβασμα των κεφαλαίων γραμμάτων με χρήση δυναμικού πίνακα,
	// αρχικά δεσμεύουμε την απαραίτητη μνήμη για τον πίνακα.
	char** array = (char**)malloc(m * sizeof(char*));
	for (int i = 0; i < m; i++)
		array[i] = (char*)malloc(n * sizeof(char));
	
	
	if( ! readCrossword(file, array, m)) // Αν υπήρξε σφάλμα κατά την αν΄άγνωση του πίνακα με τα κεφαλαία γράμματα
	{
		printf("Παρουσιάστηκε σφάλμα κατά την ανάγνωση του πίνακα των κεφαλαίων γραμμάτων\n");
		exit(4);
	}
	

	int K; // Μεταβλητή που δέχεται το πλήθος των κρυμμένων λέξεων
	if( ! readNumOfHiddenWords(file, &K)) // Αν δεν είναι επιτυχής η ανάγνωση του πλήθους των κρυμμένων λέξεων
	{
		printf("Παρουσιάστηκε σφάλμα κατά την ανάγνωση του αριθμού των κρυμμένων λέξεων από το αρχείο...\n");
		exit(5);
	}

	// Δυναμική δέσμευση του πίνακα που θα περιέχει τις κρυμμένες λέξεις
	char** hidden_words = (char**)malloc(K * sizeof(char*));
	for (int i = 0; i < K; i++)
		hidden_words[i] = (char*)malloc(m * sizeof(char));
	
	// Κλήση της συνάρτησης που δέχεται τις κρυμμένες λέξεις από το αρχέιο εισόδου 
	if( ! readHiddenWords(file, hidden_words, K)) // Αν δεν είναι επιτυχής η ανάγνωση του πίνακα με τις κρυμμένες λέξεις
	{
		printf("Παρουσιάστηκε σφάλμα κατά την ανάγνωση του πίνακα με τις κρυμμένες λέξεις\n");
		exit(6);
	}

	// Κλήση της συνάρτησης που ταξινομέι τον πίνακα με τις κρυμμένες λέξεις αλφαβητικά.
	// Δηλαδή ταξινομεί κάθε σειρά, όπου στην καθεμία βρίσκεται και η αντίστοιχη λέξη (string)
	sortHidden(hidden_words, K); 

	// Εφόσον έχουμε ταξινομήσει τον πίνακα με τις κρυμμένες λέξεις κατά αλφαβητική σειρά,
	// μπορούμε να καλέσουμε την συνάρτηση η οποία εντοπίζει τις λέξεις στον πίνακα με τα κεφαλαία γράμματα.
	findWords(array, hidden_words, m, n, K);
	
	
	fclose(file); // Κλείσιμο του αρχείου

	// Αποδέσμευση μνήμης του πίνακα με τα κεφαλαία γράμματα
	for (int i = 0; i < m; i++)
		free(array[i]);
	free(array);

	// Αποδέσμευση μνήμης του πίνακα με τις κρυμμένες λέξεις
	for (int i = 0; i < K; i++)
		free(hidden_words[i]);
	free(hidden_words);

  return 0;
}

int readDimensions(FILE* input, int* m, int* n)
{
	// Διάβασμα των διαστάσεων (M x N) που χωρίζονται με κόμμα από το αντίστοιχο αρχείο εισόδου
	if(fscanf(input, "%d,%d", m, n) == 0) // Αν υπάρχει σφάλμα κατά την ανάγνωση των διαστάσεων του πίνακα με τα γράμματα
		return 0; 

	return 1; // Αν δεν υπήρξε σφάλμα
}

int readCrossword(FILE* input, char** array, int m)
{
	// Διάβασμα των strings από το αρχείο εισόδου
	for (int i = 0; i < m; i++)
		if(fscanf(input, "%s", array[i]) == 0) // Αν υπάρχει σφάλμα κατά την ανάγνωση του πίνακα με τα γράμματα
			return 0; 

	return 1;	// Αν δεν υπήρξε σφάλμα
}

int readNumOfHiddenWords(FILE* input, int* K)
{
	// Διάβασμα αριθμού γραμμών και στηλών του π΄΄ινακα με τα κεφαλαία γράμματα
	if(fscanf(input, "%d", K) == 0) // Αν υπάρχει σφάλμα κατά την ανάγνωση του πλήθους των κρυμμένων λέξεων
		return 0; 

	return 1; // Αν δεν υπήρξε σφάλμα
}

int readHiddenWords(FILE* input, char** array, int K)
{
	// Διάβασμα των κρυμμένων λέξεων από το αρχείο εισόδου στον πίνακα hidden_words[][]	
	for (int i = 0; i < K; i++)
		if(fscanf(input, "%s", array[i]) == 0)
			return 0; // Αν υπάρχει σφάλμα κατά την ανάγνωση του πίνακα των κρυμμένων λέξεων

	return 1; // Αν δεν υπήρξε σφάλμα
}

void sortHidden(char** array, int n)
{
	// Δεσμεύουμε μνήμη για έναν πίνακα temp ο οποίος θα χρειαστεί 
	// για την ταξινόμηση του πίνακα με τις κρυμμένες λέξεις 
	char* temp = (char*) malloc(n * sizeof(char));
	for(int i = 0; i < n - 1; i++)
	{
		for(int j = i + 1; j < n; j++)
		{
			if(strcmp(array[i], array[j]) > 0)
			{
				strcpy(temp, array[i]);
				strcpy(array[i], array[j]);
				strcpy(array[j], temp);
			}
		}
	}

	free(temp);
}

void findWords(char** array, char** hidden_words, int m, int n, int K)
{
	for(int k = 0; k < K; k++) // Προσπέλαση πίνακα με τις κρυμμένες λέξεις (hidden_words[i][j])
	{
		for(int i = 0; i < m; i++)  // Προσπ΄ελαση γραμμών πίνακα κεφαλαίων γραμμάτων (array[i][j])
		{
			for(int j = 0; j < n; j++) // Προσπ΄ελαση στηλών πίνακα γραμμάτω (array[i][j])
			{
				// Τμήμα κώδικα για εύρεση λέξεων οριζόντια
				if(strncmp(array[i] + j, hidden_words[k], strlen(hidden_words[k])) == 0)
					printf("\nΒρέθηκε οριζόντια η λέξη %s. \nΠρώτο γράμμα (Γραμμή : %d , Στήλη : %d) , Τελευταίο γράμμα (Γραμμή : %d , Στήλη : %ld)\n" , hidden_words[k], i + 1 , j + 1 , i + 1 , j + strlen(hidden_words[k]) - 1);	
				
				// Τμήμα κώδικα για εύρεση λέξεων κάθετα
				int checkVertical = 1;
				if (i + strlen(hidden_words[k]) <= m)
				{
					for (int y = 0; y < strlen(hidden_words[k]); y++)
						if (array[i + y][j] != hidden_words[k][y])
							checkVertical = 0;

					if (checkVertical)
						printf("\nΒρέθηκε κάθετα η λέξη %s. \nΠρώτο γράμμα (Γραμμή : %d , Στήλη : %d) , Τελευταίο γράμμα (Γραμμή : %ld , Στήλη : %d)\n" , hidden_words[k], i + 1 , j + 1 , i + strlen(hidden_words[k]) , j + 1);	
				}
			
				// Τμήμα κώδικα για εύρεση λέξεων διαγώνια
				int checkDiagonal = 1;
				// Εδώ βρίσκουμε τις λέξεις διαγώνια από πάνω αριστερά προς τα κάτω δεξία
				if (i + strlen(hidden_words[k]) <= m && j + strlen(hidden_words[k]) <= n)
				{	
					for (int z = 0; z < strlen(hidden_words[k]); z++)
					{
						if (array[i + z][j + z] != hidden_words[k][z])
							checkDiagonal = 0;
					}
					if(checkDiagonal)
						printf("\nΒρέθηκε διαγώνια η λέξη %s. \nΠρώτο γράμμα (Γραμμή : %d , Στήλη : %d) , Τελευταίο γράμμα (Γραμμή : %ld , Στήλη : %ld)\n" , hidden_words[k], i + 1 , j + 1 , i + strlen(hidden_words[k]) , j + strlen(hidden_words[k]));
				}
			}
			
			// Ο παρακάτω βρόγχος κάνει το ίδιο με τον παραπάνω βρόγχο, αλλά για την αντίθετη κατε΄υθυνση (δεξία προς τα αριστερά).
			// Δηλαδή ξεκινάμε από τα δεξία του πίνακα και πηγαίνουμε προς τα αριστερά.
			// Μέσα στον βρόγχο αρχικοποιούμε μεταβλητές, οι οπο΄΄ιες ξεκινάνε με την τιμή 1,
			// και αν δεν βρεθεί η λέξη γίνεται 0. Έπειτα ελέγχουμε την μεταβλητή αυτή,
			// και αν είναι 1 τότε ξέρουμε ότι βρέθηκε η λέξη, οπότε τυπώνουμε αντίστοιχο μήνυμα.
			// Αυτη η λογική φαίνεται και παραπάνω για την εύρεση των λέξεων κάθετα και διαγώνια.

			for(int j = n - 1; j >= 0; j--)			
			{
				// Τμήμα κώδικα για την εύρεση των λέξεων οριζόντια (δεξία προς τα αριστερά)
				int checkReverseHorizontal = 1; 
				
				for (int x = 0; x < strlen(hidden_words[k]); x++) 
					if((j - x < 0) || (array[i][j - x] != hidden_words[k][x]))
						checkReverseHorizontal = 0;

				if (checkReverseHorizontal)
					printf("\nΒρέθηκε αντίθετα οριζόντια η λέξη %s. \nΠρώτο γράμμα (Γραμμή : %d , Στήλη : %d) , Τελευταίο γράμμα (Γραμμή : %d , Στήλη : %ld)\n", hidden_words[k], i + 1, j + 1, i + 1, j - strlen(hidden_words[k]) + 2);

				// Τμήμα κώδικα για την εύρεση των λέξεων κάθετα (κάτω προς τα πάνω)
				int checkReverseVertical = 1;

				for(int y = 0; y < strlen(hidden_words[k]); y++)
					if((i - y < 0) || (array[i - y][j] != hidden_words[k][y]))
						checkReverseVertical = 0;
				
				if(checkReverseVertical)
					printf("\nΒρέθηκε αντίθετα κάθετα η λέξη %s. \nΠρώτο γράμμα (Γραμμή : %d , Στήλη : %d) , Τελευταίο γράμμα (Γραμμή : %ld , Στήλη : %d)\n", hidden_words[k], i + 1, j + 1, i - strlen(hidden_words[k]) + 1, j + 1);

				// Τμήμα κώδικα για την εύρεση των λέξεων από κάτω δεξία προς πάνω αριστερά
				int checkReverseDiagonal = 1;
				
				for(int z = 0; z < strlen(hidden_words[k]); z++)
					if((i - z < 0 || j - z < 0) || (array[i - z][j - z] != hidden_words[k][z]))
						checkReverseDiagonal = 0;

				if(checkReverseDiagonal)
					printf("\nΒρέθηκε αντίθετα διαγώνια η λέξη %s. \nΠρώτο γράμμα (Γραμμή : %d , Στήλη : %d) , Τελευταίο γράμμα (Γραμμή : %ld , Στήλη : %ld)\n", hidden_words[k], i + 1, j + 1, (i+1) - strlen(hidden_words[k]) + 1, (j+1) - strlen(hidden_words[k]) + 1);
			}
		}
	}
}