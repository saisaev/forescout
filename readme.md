working directory content:
 readme.dm - this file
 CMakeLists.txt
 cronejob.h
 scheduler.h
 scheduler.cpp
 main.cpp
 test/test_cronejob.cpp
 test/test_scheduler.cpp


building:
1. open working directory
2. run cmake CMakeLists.txt
3. run make
5. go to ./bin directory
  - cronejobs: executable demo application
  - test_cronejob: executable cronejob unit test application
  - test_scheduler: executable scheduler unit test application

the code is compiled under g++ 5.4 with boost 1.58, ubuntu 16.04, cmake 3.5.1

classes:
1. cronejob - a task which is executed every N seconds. N is defined 
as a constructor parameter and saved as m_interval field
2. scheduler - a task manager executed stored jobs according to thier 
intervals and new timestamp received with OnNewTime(..) method.

demo application:
the application demonstrates how developed classes might be used. it runs 
a thread simulating an external time provider and calling the OnNewTime() 
method of a scheduler. the application creates 1 scheduler and adds 4 cronejobs, 
3 of them are with a context. 
output:
 a queue of crone jobs executed on a new timestamp
 {job_id #1}..{job_id #n} < timestamp (seconds)
 where n=0,1.. 

according to the requirement ¨when adding a job, a context can be specified...¨, 
a context is not a part of a job and a job might be added without a context. 
a context is defined as a callable entry: here it is function, functor or lambda.

