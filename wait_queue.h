#ifndef WaitQueue_h
#define WaitQueue_h 1

/* Name: Owen Burnett
 * Class: Operating Systems
 * Due Date: April 24, 2012
 * Program Name: disksched.cpp (Multi-level queue)
 * Description: Program implements disk scheduling algorithm using
 * first sector available.
 */

 /* file name: wait_queue.h */

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

const int DISK_SIZE = 100;  //Number of sectors for storage.

 class WaitQueue
   {
     private:
       int jobs_queued;  //Number of jobs stored in queue.
     public:
       //member data
       /* Holds job numbers of queued jobs for which number of sectors
          requested is greater than number of sectors available. */
       int* job_number;
       /* Holds number of sectors requested of queued jobs for which
          num. sectors requested is greater than num. available. */
       int* sectors_needed;

       //member functions
       //Default constructor.
       WaitQueue()
         {
           job_number = new int[DISK_SIZE];
           sectors_needed = new int[DISK_SIZE];
           jobs_queued = 0;  //No jobs have been queued.

           int index = 0;
           for(; index < DISK_SIZE; index++)
             {
               job_number[index] = 0;
               sectors_needed[index] = 0;
             }  //end for
         }  //end default constructor
       
       //Destructor 
       ~WaitQueue()
         {
           delete job_number;
           delete sectors_needed;
         }  //end destructor

       /* Add the job whose job number is job_number to wait queue.
          @param job_number, job number of job to add to wait queue.
          @param sectors_needed, number of sectors needed by job. */
       void Enqueue(int job_number, int sectors_needed)
         {
           this->job_number[jobs_queued] = job_number;
           this->sectors_needed[jobs_queued] = sectors_needed;
         }  //end enqueue

       /* Delete job with job number equal to job_number from queue.
          @param job_number, job number of job to be deleted. */
       void Dequeue(int job_number)
         {
           int index = 0;
           for(; index < DISK_SIZE; index++)
             {
               if(this->job_number[index] == job_number && index < 99)
                 {  //The job has been found in the wait queue.
                   for(; index < DISK_SIZE - 1; index++)
                     {
                       this->job_number[index] = this->job_number[index+1];
                       this->sectors_needed[index] = this->sectors_needed[index+1];
                       if(this->job_number[index] == 0) break;
                     }  //end for
                 }  //end if
             }  //end for
         }  //end dequeue

       bool In_Queue(int job_number)
         {
           int index = 0;
           for(; index < jobs_queued; index++)
             {
               //Return true if job is in the queue.
               if(this->job_number[index] == job_number) return true;
             }  //end for
           //The job was not found in the queue.
           return false;
         }  //end InQueue

       /* Print each job number in the wait queue with its number of
          sectors needed.
          @param o, output stream to print to. */
       void Print_Wait_Queue(ostream& o)
         {
           int index = 0;
           o << "wait queue";
           for(; index < jobs_queued; index++)
             {
               if(index % 10 == 0) o << endl;                         
               if(this->job_number[index] == 0) break;
               //Print job number and sectors needed for each job.
               o << "   " << this->job_number[index] << "  ";
               o << this->sectors_needed[index];
             }  //end for
         }  //end Print_Wait_Queue

       int Get_Jobs_Queued(void)
         {
           return jobs_queued;
         }  //end Get_Jobs_Queued

       void Set_Jobs_Queued(int current_jobs_queued)
         {
           jobs_queued = current_jobs_queued;
         }  //end Set_Jobs_Queued
 }; //end class Wait_Queue
#endif
