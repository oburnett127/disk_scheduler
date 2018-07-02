#ifndef DiskScheduler_h
#define DiskScheduler_h 1

/* Name: Owen Burnett
 * Class: Operating Systems
 * Due Date: April 24, 2012
 * Program Name: disksched.cpp (Multi-level queue)
 * Description: Program implements disk scheduling algorithm using
 * first sector available.
 */

 /* file name: disk_sched.h
    File disk_sched.h defines class DiskScheduler
  */

//C++ standard include files.
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

//Application include files.
#include "wait_queue.h"

 /* Memory map is 10 rows by 10 columns. */
 const int MAP_SIZE = 10;

 class DiskScheduler
   {
       WaitQueue* wait_queue;
       /* Shows sectors on disk allocated with job number, and which
          sectors are available for allocation. */
       int* memory_map;
       /* Number of sectors allocated to a given job. */
       int* job_sectors;
       /* Number of sectors available for storage allocation. */
       int sectors_available;
       char command;  //'a' allocate, 'd' deallocate, 'q' query.
       int job_number;  //Number of the current job.
       int sectors_needed;  //Number of sectors the job needs.
       /* Number of sectors allocated to the current job. */
       int number_job_sectors;
                                      
     public:
       //Default constructor.
       DiskScheduler()
         {
           command = ' ';
           job_number = 0;
           sectors_needed = 0;
           number_job_sectors = 0;
           sectors_available = DISK_SIZE;  //All sectors are available
           memory_map = new int[DISK_SIZE];
           job_sectors = new int[DISK_SIZE];
           wait_queue = new WaitQueue();
           
           /* Initialize sectors_allocated, and memory_map
             with 0s. */
           int index = 0;
           for(; index < DISK_SIZE; index++)
             {
               job_sectors[index] = 0;
               memory_map[index] = 0;
             }  //end for
         } //end default constructor DiskScheduler

       //Destructor.
       ~DiskScheduler()
         {
           delete memory_map;
           delete job_sectors;
           delete wait_queue;   
         }  //end DiskScheduler destructor

       /* Display allocated/deallocated sectors with job numbers. The
          memory map has 10 rows and 10 columns, 100 sectors total.
          @param o, output stream to print to. */
       void Print_Memory_Map(ostream& o)
         {
           int row = 0;  //Current row in memory_map.
           int column = 0;
           int index = 0;
           o << "disk storage map" << endl;
           //Print all rows in memory map.
           for(; row < MAP_SIZE; row++)
             {
               column = 0;
               for(; column < MAP_SIZE; column++)
                 {  //Print the job number.
                   if(memory_map[index] > 9)  //Number has two digits.
                     o << "   ";
                   else  //number has one digit
                     o << "    ";
                   o << memory_map[index];
                   index++;
                 }  //end for
               o << endl;               
             }  //end for
         }  //end Print_Memory_Map

       void Print_Wait_Queue(ostream& o)
         {
           wait_queue->Print_Wait_Queue(o);
         }  //end PrintWaitQueue

       /* Print the list of sectors that have been allocated to the
          current job.
          @param o, the output stream to which list is written. */
       void Print_Job_Sectors(ostream& o)
         {
           int index = 0;
           if(number_job_sectors > 0) o << "sectors allocated" << endl;
           for(; index < number_job_sectors; index++)
             {
               if(index % 10 == 0 && index != 0) o << endl;
               if(job_sectors[index] > 9)
                 o << "   ";  //The number has two digits.
               else
                 o << "    "; //The number has one digit.
               //end if
               o << job_sectors[index];
             }  //end for
           if(number_job_sectors > 0) o << endl;
         }  //end Print_Sectors_Allocated

       //Get the number of sectors available for allocation.
       int Get_Sectors_Available(void)
         {
           return sectors_available;
         }  //end Get_Sectors_Available

       /* Print number of sectors available for allocation to jobs
          by the disk scheduler.
          @param o, output stream to print to. */
       void Print_Sectors_Available(ostream& o)
         {
           o << "available " << sectors_available << endl;
         }  //end Print_Sectors_Available

       /* Set the number of sectors available in the disk.
          @param current_sectors_available, number of sectors
            available for storage allocation. */
       void Set_Sectors_Available(int current_sectors_available)
         {
           sectors_available = current_sectors_available;
         }  //end Set_Sectors_Available

       char Get_Command()
         {
           return command;
         }  //end Get_Command

       int Get_Number_Job_Sectors(void)
         {
           return number_job_sectors;
         }  //end Get_Number_Job_Sectors

       /* Read command ('a' for allocate, 'd' for deallocate, 'q' for query),
          job number of current job, and sectors needed from input stream. 
          @param i, input stream to read from. */
       void Read_Job(istream& i)
         {
           i >> command;
           i >> job_number;
           i >> sectors_needed;
         }  //end Read_Job

       /* Print command, job number of current job, and sectors needed to
          output stream.
          @param o, output stream to print to. */
       void Write_Job(ostream& o)
         {
           o << command << "    ";
           o << job_number << "      ";
           o << sectors_needed << endl;
         }  //end Write_Job

       /* This disk scheduler uses the first sector available algorithm.  The
          first available sector that is found in the memory map is allocated
          to the job if there are at least as many sectors free as the number
          of sectors requested by the job. When the first free sector is
          allocated the next free sector is searched for and allocated to the
          job.
          @param o, output stream to print message to.
          @return 0 if enough free sectors are available for allocation to the
             job or 1 if there are not enough sectors available. */
       int Allocate_Storage(ostream& o)
         {
           //No sectors have been allocated to current job.
           number_job_sectors = 0;

           if(sectors_available >= sectors_needed)
             {  /* There are enough free sectors available for allocation to
                  the current job. */
               int index = 0;
               for(; index < DISK_SIZE; index++)
                 {
                   //Stop allocating when request has been met.
                   if(number_job_sectors >= sectors_needed) break;
                   if(memory_map[index] == 0)
                     {  //The current sector is available for allocation.
                       memory_map[index] = job_number;
                       number_job_sectors++;
                       sectors_available--;  //One less sector is available.
                       /* Store sector number in array of sectors allocated
                          for the current job. */
                       job_sectors[number_job_sectors-1] = index + 1;
                     }  //end if
                 }  //end for
               return 0;
             }
           else
             { /* There are not enough free sectors available for allocation
                 to the current job. */
               if(wait_queue->In_Queue(job_number) == false)
                 {
                   o << "request deferred" << endl;
                   /* Add job number and sectors needed for current job to their
                      respective queues. */
                   wait_queue->Enqueue(job_number,sectors_needed);
                   wait_queue->Set_Jobs_Queued(wait_queue->Get_Jobs_Queued()+1);
                 }
               return 1;
             }  //end if
         }  //end Allocate_Storage
       
       /* Remove job from memory or from wait queue if job is not in memory.
          This frees storage for use by other jobs.
          @param o, output file to which output should be written. */
       void Deallocate_Storage(ostream& o)
         {
           /* Deallocate space for the job number given. */
           int index = 0;
           bool job_in_memory = false;  /* True if current job was located in
                                           memory. */
           for(; index < DISK_SIZE; index++)
             {
               if(memory_map[index] == job_number)
                 { /* The current sector is allocated to the job number for
                      which storage is being deallocated. */
                   memory_map[index] = 0;  //Free the sector.
                   sectors_available++;  //One more sector is available.
                   job_in_memory = true;
                 }  //end if
             }  //end for
           if(job_in_memory == false)
             {  //The job was not found in memory, delete job from wait queue.
                wait_queue->Dequeue(job_number);
             }  //end if
         }  //end Deallocate_Storage

       /* Try to allocate items in queue if they can be allocated. */
       void Service_Queue_Items(ostream& o)
         {
           //Result of trying to allocate storage for current queue item.
           bool allocate_result = false;
           int index = 0;  //Used to try to allocate jobs in queue.
           int job = 0;      //Job number of queue item.
           int request = 0;  //Sectors requested by queue item.
           for(; index < DISK_SIZE; index++)
             {  
                job = wait_queue->job_number[index];
                request = wait_queue->sectors_needed[index];
                if(job != 0)
                  {
                    job_number = job;  //Allocate function needs to access job number.
                    sectors_needed = request;
                    allocate_result = Allocate_Storage(o);
                    if(allocate_result == false)
                      { //The job has been allocated.
                        //Remove job from queue. Move all other jobs up in queue.
                        wait_queue->Dequeue(job);
                        wait_queue->Set_Jobs_Queued(wait_queue->Get_Jobs_Queued()-1);
                        /* The job that is after the allocated job should not be
                        skipped on the next iteration. */
                        index--;
                      }  //end if
                  }  //end if
             }  //end for
         }  //end Service_Queue_Items
   };  //end class DiskScheduler
#endif
