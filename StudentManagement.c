#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/types.h>


 struct student{
    int id;
    char *name;
    float marks;
};

typedef struct student student;
char filename[50];
student* students = NULL;
int StudentCount=0;
int ismodified=0;
int IdLength = 2;      // Default ID length ("ID")
int NameLength = 4;    // Default Name length ("Name")
int MarksLength = 5;   // Default Marks length ("Marks")


void freeStudentsMemory() {
    for(int i=0; i<StudentCount; i++) {
        free(students[i].name);
    }
    free(students);
}

void updateColumnWidth()
{

    for(int i=0;i<StudentCount;i++)
    {
      int idWidth=snprintf(NULL,0,"%d",students[i].id);
      if(idWidth>=IdLength) IdLength=idWidth;

      int nameWidth=strlen(students[i].name);
      if(nameWidth>=NameLength) NameLength=nameWidth;

    }
}

void printdash(FILE*file)
{
 
     int sNoWidth = snprintf(NULL, 0, "%d", StudentCount) + 1;
   int totalWidth=5+sNoWidth+IdLength+NameLength+12;
     for(int i=0;i<totalWidth;i++)
     fprintf(file,"-");

     fprintf(file,"\n");
}

void clearbuffer()
{
  int c;
    while((c=getchar())!='\n'&&c!=EOF);
}

void addTxtExtension() {
    if (strstr(filename, ".txt") == NULL) {
        strcat(filename, ".txt");
    }
}

// Utility function to trim trailing whitespace
void trimTrailingSpaces(char *str) {
    int index, i;
    index = -1;

    // Find last non-white space character
    for (i = 0; str[i] != '\0'; ++i) {
        if (!isspace(str[i])) {
            index = i;
        }
    }
    // Mark the new end of the string
    str[index + 1] = '\0';
}

int validateNumber(char *input) {
    // Check if the string is empty
    if (input[0] == '\0') return 0;

    // Loop through each character to validate it's a digit
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    return 1;
}

int validateName(char *input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isalpha(input[i]) && input[i] != ' ') {
            return 0; // Contains invalid characters
        }
    }
    return 1; // Valid name
}

int validateMarks(float marks) {
    return (marks >= 0 && marks <= 100);
}

void lowercase(){
    for(int i=0;i<StudentCount;i++){
        for(int j=0;j<strlen(students[i].name);j++){
        if(students[i].name[j]>='A' && students[i].name[j]<='Z'){
            students[i].name[j]+=32;
        }
    }
  }
}
// function to capitize searchterm
char* capitalizeSearchTerm(char* searchTerm){
   for(int i=0;i<strlen(searchTerm);i++){
    if((i==0 && searchTerm[i]>='a' && searchTerm[i]<='z') ||
       (i>0  && searchTerm[i-1]==' ' && searchTerm[i]>='a' && searchTerm[i]<='z')){
        searchTerm[i]-=32;
       }
      }
      return searchTerm;
    }


void capitalize(){
    for(int i=0;i<StudentCount;i++){
        for(int j=0;j<strlen(students[i].name);j++){
        if((j==0 && students[i].name[j]>='a' && students[i].name[j]<='z' )
        || (j>0 && students[i].name[j-1]==' ' && students[i].name[j]>='a' && students[i].name[j]<='z')){
        students[i].name[j]-=32;
    }
  }
 }
}


void loadStudentFromFile() {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file. Starting with an empty list.\n");
        return;
    }

    // Dynamically read and discard the first two header lines
    char* headerline = NULL;
    size_t len = 0;
    for (int i = 0; i < 2; i++) {
        if (getline(&headerline, &len, file) == -1) {
            printf("Error reading header line %d.\n", i + 1);
            free(headerline);
            fclose(file);
            return;
        }
    }
    free(headerline);  // Free after header read

    student temp;
    int sNo;

    char* nameBuffer = NULL;
    size_t nameBufferSize = 0;

    while (!feof(file)) {
        nameBufferSize = 128;
        nameBuffer = malloc(nameBufferSize);
        if (nameBuffer == NULL) {
            printf("Memory allocation failed for nameBuffer.\n");
            exit(1);
        }

        int result = fscanf(file, " %d | %d | %[^|]| %f\n", &sNo, &temp.id, nameBuffer, &temp.marks);
        if (result != 4) {
            free(nameBuffer);
            break;
        }

        trimTrailingSpaces(nameBuffer);

        temp.name = malloc(strlen(nameBuffer) + 1);
        if (temp.name == NULL) {
            printf("Memory allocation failed for temp.name.\n");
            free(nameBuffer);
            exit(1);
        }
        strcpy(temp.name, nameBuffer);
        free(nameBuffer);

        students = realloc(students, (StudentCount + 1) * sizeof(student));
        if (students == NULL) {
            printf("Memory allocation failed for students array.\n");
            exit(1);
        }

        students[StudentCount++] = temp;
    }

    fclose(file);
}

void sortAndSaveToFile()
{
  FILE* file= fopen(filename,"w");
  if(file==NULL)
  {
    printf("error opening file.\n");
    return;
  }

  updateColumnWidth();

  // Calculate the width of S.No based on StudentCount
    int sNoWidth = snprintf(NULL, 0, "%d", StudentCount) + 1;

     fprintf(file, "%-*s | %-*s | %-*s | %-*s\n",
      sNoWidth-1, "S.No",
       IdLength, "ID", 
       NameLength, "Name", 
       MarksLength, "Marks");

      printdash(file);
 
  // lowercase name
      lowercase();
  // sorting according to name
  for(int i=0;i<StudentCount-1;i++)
  {
    for(int j=0;j<StudentCount-i-1;j++)
    {
      if(strcmp(students[j].name,students[j+1].name)>0)
      {
        student temp=students[j];
        students[j]=students[j+1];
        students[j+1]=temp;
      }
    }
  }

    // capitalizing name before saving to file
    capitalize();

    // Writing sorted student data to the file
    for (int i = 0; i < StudentCount; i++)
    {
        fprintf(file, " %-*d | %-*d | %-*s | %.2f\n",sNoWidth+1,i+1,IdLength, students[i].id,NameLength, students[i].name, students[i].marks);
    }

     printdash(file);

     fflush(file);
     fclose(file);
     printf("Data Saved in File.\n");
  
}

void addStudent()
{
  student newStudent;
  char *IDinput=NULL;
  size_t idBufferSize=0;
  int idExists=0;

  do {
        printf("\nEnter student ID (numbers only): ");// enter student ID
        ssize_t len=getline(&IDinput,&idBufferSize, stdin);

        if(len==-1)
        {
          printf("Error in reading input.\n");
          free(IDinput);
          return;
        }
    
        IDinput[strcspn(IDinput, "\n")] = '\0'; // Removing newline character

        if (!validateNumber(IDinput)) {
            printf("Invalid ID! Please enter a valid numeric value.\n");
        }else{
          newStudent.id=atoi(IDinput);

          //check for unique ID
          idExists=0;
          for(int i=0;i<StudentCount;i++)
          {
            if(students[i].id==newStudent.id)
            {
              idExists=1;
              break;
            }
          }

          if(idExists)
          {
            printf("\nID already exists! Please enter a unique ID.\n");
          }

        }

    } while(idExists|| !validateNumber(IDinput));


  // for name
  char*nameIP=NULL;
  size_t namaebuffersize=0;

   do{
      printf("\bEnter student name(alphabets and space only): ");
      ssize_t nlen=getline(&nameIP,&namaebuffersize,stdin);
       if(nlen==-1) {
        printf("Error reading input.\n");
        free(nameIP);
       return;
      }

      nameIP[strcspn(nameIP, "\n")] = '\0';

     if (!validateName(nameIP)) {
         printf("Invalid Name! Please enter only alphabets and spaces.\n");
        }

    }while(!validateName(nameIP));

  newStudent.name=malloc(strlen(nameIP)+1);
  if(newStudent.name==NULL)
  {
    printf("Memory allocation failed.\n");
    free(nameIP);
    return;
  }
  strcpy(newStudent.name,nameIP);
  free(nameIP);


  // Marks Input
    do {
        printf("Enter marks (0 to 100): ");
        scanf("%f", &newStudent.marks);
        clearbuffer();
        
        if (!validateMarks(newStudent.marks)) {
            printf("Invalid Marks! Please enter a value between 0 and 100.\n");
        }
    } while (!validateMarks(newStudent.marks));

  students = realloc(students,(StudentCount+1)*sizeof(student));
    if(students==NULL)
    {
      printf("Memory allocatiion failed.\n");
      exit(1);
    }
  
  students[StudentCount++] = newStudent;

  ismodified=1;
  printf("Student Record has been updated.\n");
}

void displayStudents()
{
  FILE* file=fopen(filename,"r");
  if(file==NULL)
  {
    printf("Error opening file.\n");
    return;
  }
  char *line=NULL;
  ssize_t len=0;
  ssize_t read;
  
  printf("\nDisplaying All Student Record \n");

  while(read= getline(&line,&len,file)!=-1)
  {
    printf("%s",line);
  }
  free(line);
  fclose(file);
  return;
}

int searchStudents()
{
  char *searchTerm=NULL;
  ssize_t len=0;
  int found = 0;
  int isNumericSearch=1;
 
  getline(&searchTerm,&len,stdin);// user input for searchterm
  searchTerm[strcspn(searchTerm,"\n")]='\0';

  for(int i=0;searchTerm[i]!='\0';i++)// checking if searchterm is numeric
  {
    if (!isdigit(searchTerm[i]))
    {
      isNumericSearch=0;
      break;
    }
  }

  printf("\nsearching for %s\n",searchTerm);
    // trailing space for searching.
  if(!isNumericSearch)
 {
     searchTerm=capitalizeSearchTerm(searchTerm);
  trimTrailingSpaces(searchTerm);
 }
  for(int i=0;i<StudentCount;i++)
  {
    if((isNumericSearch&&atoi(searchTerm)==students[i].id)||(!isNumericSearch&&strcmp(searchTerm,students[i].name)==0))
    {

      found=1;
      printf("Student record found for %s.\n",searchTerm);
      printf("ID: %d, Name: %s, Marks: %.2f\n",students[i].id,students[i].name,students[i].marks);
    }
  }
  free(searchTerm);

  if(!found)
  {
    printf("No record found for %s.\n",searchTerm);
    return 0;
  }

  else 
  return 1;
}

void updateStudents()
{
    int found = 0;
    int isNumericSearch = 1;
    char *searchTerm=NULL;
    ssize_t len=0;

    printf("enter student ID or Name to update: ");
    getline(&searchTerm,&len,stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';

    if(!validateNumber(searchTerm)){
      searchTerm=capitalizeSearchTerm(searchTerm);
    }


    // Search through the array of students
for (int i = 0; i < StudentCount; i++)
    {
        if ((validateNumber(searchTerm) && students[i].id == atoi(searchTerm)) || 
            (!validateNumber(searchTerm) && strcmp(students[i].name, searchTerm) == 0))
        {
            found = 1;
            printf("\nStudent found:\n");
            printf("ID: %d, Name: %s, Marks: %.2f\n", students[i].id, students[i].name, students[i].marks);

            char *IDinput=NULL;
            ssize_t idlen=0;
            int idExists=0;
          //enter new id 

        do {
        printf("\nEnter student New ID (numbers only): ");
        getline(&IDinput,&idlen,stdin);
        IDinput[strcspn(IDinput, "\n")] = '\0'; // Removing newline character

        if (!validateNumber(IDinput)) {
            printf("Invalid ID! Please enter a valid numeric value.\n");
        }else{

          //check for unique ID

          idExists=0;
          for(int j=0;j<StudentCount;j++)
          {
            if(students[j].id==atoi(IDinput)&&students[j].id!=students[i].id)
            {
              idExists=1;
              break;
            }
          }

          if(idExists)
          {
            printf("\nID already exists! Please enter a unique ID.\n");
          }
          else{
            students[i].id=atoi(IDinput);
            break;
          }
        }
       } while (!validateNumber(IDinput)||atoi(IDinput) <=0||idExists);
         free(IDinput);

              // Student new Name 
              char *newName=NULL;
              ssize_t nlen=0;
    do {
        printf("Enter student New name (alphabets and spaces only): ");
        getline(&newName,&nlen,stdin);
        newName[strcspn(newName,"\n")]='\0';

        if (!validateName(newName)) {
            printf("Invalid Name! Please enter only alphabets and spaces.\n");
        }
    } while (!validateName(newName));

    //reallocate memory for new name
    students[i].name=realloc(students[i].name,strlen(newName)+1);
    if(students[i].name==NULL)
    {
      printf("Memory allocation failed.\n");
      exit(1);
    }
    strcpy(students[i].name,newName);
    free(newName);

     // Marks Input
    do {
        printf("Enter New marks (0 to 100): ");
        scanf("%f", &students[i].marks);
        clearbuffer();
        
        if (!validateMarks(students[i].marks)) {
            printf("Invalid Marks! Please enter a value between 0 and 100.\n");
        }
    } while (!validateMarks(students[i].marks));

    break;
  }
 }

  if(found)
  {
    
            printf("\nStudent record updated successfully.\n");
            
  }
   

    if (!found)
    {
        printf("\nNo record found for %s.\n", searchTerm);
    }

    // Save the changes to the array
    ismodified=1;

  }

void deleteStudents()
{
  char *deleteTerm=NULL;
  ssize_t dlen=0;
  int found=0;
  int isNumericSearch=1;

  printf("Enter Student ID or Name to Delete: ");
  getline(&deleteTerm,&dlen,stdin);
  deleteTerm[strcspn(deleteTerm,"\n")]='\0';
  
  for(int i=0;deleteTerm[i]!='\0';i++)
  {
    if(!isdigit(deleteTerm[i]))
    {
      isNumericSearch=0;
      break;
    }
  }
  // capitalizing delete term
  if(!isNumericSearch){
    deleteTerm=capitalizeSearchTerm(deleteTerm);
  }

  // search and delete logic
  for(int i=0;i<StudentCount;i++)
  {
    if((isNumericSearch&&students[i].id==atoi(deleteTerm))||
       (!isNumericSearch&&strcmp(students[i].name,deleteTerm)==0))
       {
        found=1;
        //shift record
        for(int j=i;j<StudentCount-1;j++)
        {
          students[j] = students[j+1];
        }
        StudentCount--;
        //reallocate memory
        if(StudentCount>0)
        {
          student *temp=realloc(students,StudentCount*sizeof(student));
          if(temp==NULL)
          {
            printf("Memory reallocation failed.\n");
            free(deleteTerm);
            exit(1);
          }
          students=temp;
        }  else{
            free(students);
            students=NULL;
          }

        printf("\nStudent record deleted successfully.\n");

        //save the updated list to the array
        ismodified=1;
        break;
       }
  } 

  if(!found){
    printf("\nNo record found for %s.\n",deleteTerm);
  }
}

void  displayStudentsbymarks()
        {
          if(StudentCount==0)
          {
            printf("No student records available.\n");
            return;
          }

          student* tempstudents= malloc(StudentCount*sizeof(student));
          if(!tempstudents)
          {
            printf("Memory allocation failed.\n");
            return;
          }

          //deep copy
          for(int i=0;i<StudentCount;i++){
            tempstudents[i].id=students[i].id;
             tempstudents[i].marks = students[i].marks;

             tempstudents[i].name = malloc(strlen(students[i].name) + 1);  // memory allocation

             if (!tempstudents[i].name) {
                  printf("Memory allocation failed for tempstudents[i].name.\n");
                  // Free previously allocated names
                 for (int k = 0; k < i; k++) {
                 free(tempstudents[k].name);
                 }
                 free(tempstudents);
                 return;
                }
             strcpy(tempstudents[i].name, students[i].name);
          }

          //sorting
          for(int i=0;i<StudentCount;i++)
          {
            for(int j=0;j<StudentCount-i-1;j++)
            {
              if(tempstudents[j].marks>tempstudents[j+1].marks)
              {
                student tmp = tempstudents[j];
                tempstudents[j]=tempstudents[j+1];
                tempstudents[j+1]=tmp;

              }
            }
          }

          //print headers

          printf("\nStudents Sorted by Marks (low to high):\n");
          int sNoWidth=sprintf(NULL,0,"%d",StudentCount)+1;
          updateColumnWidth();

         printf("%-*s | %-*s | %-*s | %-*s\n",
                sNoWidth - 1, "S.No",
                IdLength, "ID",
                NameLength, "Name",
                MarksLength, "Marks");
    
        int totalWidth=5+sNoWidth+IdLength+NameLength+12;
        for(int i=0;i<totalWidth+2;i++)
        printf("-");
        printf("\n");


       //print data
      for (int i = 0; i < StudentCount; i++) {
        printf(" %-*d | %-*d | %-*s | %.2f\n",
            sNoWidth + 3, i + 1,
            IdLength, tempstudents[i].id,
            NameLength, tempstudents[i].name,
            tempstudents[i].marks);
        }

     for(int i=0;i<totalWidth+2;i++)
     printf("-");

     printf("\n");

     for(int i=0;i<StudentCount;i++)
     {
      free(tempstudents[i].name);
     }

     free(tempstudents);
 }

int main()
{
  
  printf("enter file name: ");
  fgets(filename,sizeof(filename),stdin);
  filename[strcspn(filename,"\n")]='\0';

  addTxtExtension();
 
  loadStudentFromFile();

    int choice;
    do{
      printf("\n--- Student Management System ---\n");
      printf(" 1. Add Student\n 2. Display All Students\n 3. Search for a Student\n 4. Update Student Details\n 5. Delete a Student\n 6. Display Students record Sorted by Marks\n 7. Exit\n Enter your choice: ");
      scanf("%d",&choice);
      clearbuffer();

      switch(choice)
      {
        case 1:
        addStudent();
        break;
        
        case 2:
            if(ismodified){
            sortAndSaveToFile();
            }
             displayStudents();
             break;
        
        case 3:
         printf("enter Student ID or Name to search: ");
        searchStudents();
        break;
        
        case 4:
        updateStudents();
        break;

        case 5:
        deleteStudents();
        break;

        case 6:
        displayStudentsbymarks();
        break; 

        case 7:
        if(ismodified)
        sortAndSaveToFile();
        printf("\nThank you for using the Student Management System!\n");
        freeStudentsMemory();
        exit(1);
        break;

        default:
        printf("Invalid choice.Try again.\n"); 
      }
      }while(1);


      free(students);

       return 0;
}
