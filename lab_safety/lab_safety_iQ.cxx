// g++ -pthread -Wall -O3 -std=c++14 test.cxx -o test -lL412B -lftdi


#include <L412B/driver/include/Driver.h>
#include <L412B/driver/include/Data.h>
#include <vector>
#include <iostream>
#include <memory>
#include <typeinfo>

int main (void)
{
	
	auto dev_list = L412B::Driver::scan();

	if (dev_list.empty())
	{
		std::cout << "No QDA device found." << std::endl;
		return (-1);
	}

	auto serialNo = dev_list[0].serialNo();
	std::cout << "Serial No:" << serialNo <<std::endl;
        
    auto driver = std::make_unique<L412B::Driver>();
    driver->connect (serialNo);
	driver->start_acquisition ();


    while(1)
    {
        auto live_data = driver->live_data();
        if (live_data.empty()) continue;


	    	for(int i= 0; i< (int)live_data.size() ; i++ )
	    	{
	    		double intpart = 0.0 ;
	        	if(std::modf(live_data[i].time()*1.0, &intpart)  == 0.00)
		        	{
		        		if(live_data[i].chn()==0)
		        		{
		        			std::cout <<  "Time : \a" << live_data[i].time() << "\t Chn 1:  " << live_data[i].data() ;
		        			if(live_data[i].data() > .7)
		        			{
		        				std::cout << "\t \t ******DANGER******" << std::endl ;
		        				continue;
		        			}

		        		}
		        		if(live_data[i].chn()==1)
		        		{
		        			std::cout << "\t Chn 2:  " << live_data[i].data() << std::endl;
		        			if(live_data[i].data() > .7)
		        			{
		        				std::cout << "\t \t ******DANGER******" << std::endl ;
		        				continue;
		        			}

		        		}
		        		// if(live_data[i].chn()==2)
		        		// {
		        		// 	std::cout <<  "Time : \a" << live_data[i].time() << "\t Chn 3:  " << live_data[i].data() << std::endl;
		        		// }
		        		// if(live_data[i].chn()==3)
		        		// {
		        		// 	std::cout <<  "Time : \a" << live_data[i].time() << "\t Chn 4:  " << live_data[i].data() << std::endl;
		        		// }


/*
		        		// if(live_data[i].chn()==0)
		        		// {
			        	// std::coutd << "Time : \a" << live_data[i].time() << "\t Chn 1:  " << live_data[i].data() \
			        	// 											 << "\t Chn 2:  " << live_data[i+1].data() \
			        	// 											 << "\t Chn 3:  " << live_data[i+2].data() \
			        	// 											 << "\t Chn 4:  " << live_data[i+3].data() \
			        	// 											 <<std::endl\;
			        	// i=i+4;
			        	
			       		// }*/
		        	}

	   		}
	   		
    }
    
	driver->stop_acquisition ();
	driver->disconnect();
    
	return 0;
}
