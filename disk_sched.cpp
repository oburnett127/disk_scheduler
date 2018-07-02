/* Name: Owen Burnett
 * Class: Operating Systems
 * Due Date: April 24, 2012
 * Program Name: disksched.cpp (Multi-Level Queue)
 * Description: Program implements disk scheduling algorithm
 * using first sector available.
 */

/* The disk scheduling algorithm used in this program was borrowed
 * from Dr. Bill McDaniel's website. The algorithm is viewable at
 * the following web page:
 * http://cs2.uco.edu/~mcdaniel/opsys/disk-scheduler.html
 */

 /* file name: disk_sched.cpp */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

#include "disk_sched.h"

using namespace std;

/* Display message if too many command line arguments were entered.
   @param max, the maximum number of arguments permitted.
   @param actual, the number of arguments the user of the program entered.
 */
struct CommandLineException
  {
    CommandLineException(int max, int actual)
      {
        cout << endl << "Too many command line arguments."
             << endl;
        cout << "A maximum of " << max << "arguments are "
             << "permitted." << endl;
        cout << actual << "arguments were entered." << endl;
      }  //end constructor CommandLineException
  };  //end struct CommandLineException
//-------------------------------------------------------------
/* Display message if the input or output file could not be opened.
   @param char* fn, the filename of the file that could not be opened. */
struct FileException
  {
    FileException(const char* fn)
      {
        cout << endl << "File " << fn << " could not be "
             << "opened." << endl;
      }  //end constructor FileException
  };  //end struct FileException
//-------------------------------------------------------------
/* This function schedules jobs in a multi-level queue system.
   When the command 'a' is read from input file storage should
   be allocated, 'd' means storage is to be deallocated, 'q'
   means a query is to be performed. Scheduling is performed
   using First Sector Available (FSA). Units and the units are
   allocated one at a time.
   @param i, input file handler.
   @param o, output file handler.
   @param command, specifies either allocation, dealloc, or query.
   @param job_number, the number of the job to be scheduled.
   @param space_needed, number of disk sectors required by job.
 */
void disk_scheduler(istream& i, ostream& o)
  {
    DiskScheduler disk_sched;
    while(!i.eof())
      {
        //Read and write command, job number, sectors_needed.
        disk_sched.Read_Job(i);
        if(i.eof()) break;
        disk_sched.Write_Job(o);
        
        switch(disk_sched.Get_Command())
          {
            case 'a':  //Disk storage should be allocated.
              //Allocate storage for this job.
              disk_sched.Allocate_Storage(o);
              //Print locations of units used by job.
              disk_sched.Print_Job_Sectors(o);
              if(disk_sched.Get_Number_Job_Sectors() > 0)
                { //Storage was allocated to the job.
                  //Print memory map of space used and space available.
                  disk_sched.Print_Memory_Map(o);
                }  //end if
              //Print wait queue of job numbers and space needed.
              disk_sched.Print_Wait_Queue(o);
              break;
            case 'd':  //Disk storage should be deallocated.
              //Deallocate space for job number given.
              disk_sched.Deallocate_Storage(o);
              //Service items in the wait queue that can be serviced.
              disk_sched.Service_Queue_Items(o);
              //Print memory map of space used and space available.
              disk_sched.Print_Memory_Map(o);
              //Print wait queue containing job numbers and sectors needed.
              disk_sched.Print_Wait_Queue(o);
              break;
            case 'q':  //A query is to be performed.
              //Print the memory map of space used and space available.
              disk_sched.Print_Memory_Map(o);
              //Print wait queue of job numbers and space needed.
              disk_sched.Print_Wait_Queue(o);
              break;
          }  //end switch
        //Print number of sectors available for allocation to jobs.
        o << endl << "available " << disk_sched.Get_Sectors_Available() << endl << endl << endl;
      }  //end while
  }  //end function disk_scheduler
//-------------------------------------------------------------
int main(int argc, char* argv[])
  {
    try
      {
        char ifn[255], ofn[255];  //hold input and output file names

        switch(argc)
          { /* User supplied neither input file name nor output
               file name. */
            case 1:  
              cout << "Enter the input file name: ";
              cin >> ifn;
              cout << "Enter the output file name: ";
              cin >> ofn;
              break;
            /* User supplied input filename but not output. */
            case 2:
              strcpy(ifn, argv[1]);
              cout << "Enter the output file name: ";
              cin >> ofn;
              break;
            /* User supplied both input and output file name. */
            case 3:
              strcpy(ifn, argv[1]);
              strcpy(ofn, argv[2]);
              break;
            /* The user supplied too many command line
               arguments. */
            default:
              throw CommandLineException(2, argc - 1);
              break;
          }  //end switch

          ifstream i(ifn);  //assign input filename to input file handler
          if(!i) throw FileException(ifn); //input file hand. didn't open
          ofstream o(ofn); //assign output filename to output file handler
          if(!o) throw FileException(ofn); //output file hand. didn't open
          
          /* Start reading commands from input file: allocate, deallocate,
             and query, process commands while managing the queue. */
          disk_scheduler(i, o);
          
          //close input and output files
          o.close();
          i.close();
      } 
      catch(...) 
      {
        cout << "Program terminated." << endl;
        exit(EXIT_FAILURE);
      }  //end try catch

    return 0;

  }  //end main
