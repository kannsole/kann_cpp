/*
This code demonstrates the use of C++ condition_variable for the purpose of event handling in multithreaded environment.
ues case: FBI wants to rescue a child captured by the terrorists. FBI sends one agent to save the child and four soldiers to destroy the terrorist location.
soldiers cover the area and waits for the signal from the agent. once the child is saved, agent turns the signal on. upon seeing the signal, soldiers starts attacking.
*/


#include<iostream>
#include<thread>
#include <mutex>
#include<condition_variable>
#include<functional>
#include<vector>

struct Signal{

	bool m_SignlStatus{false};
	std::mutex m_mtx;
	std::condition_variable m_cv;
	bool get_Signal_Status(){return m_SignlStatus; }
	void blow(){
		std::cout<<"signal is ON"<<std::endl;
		m_SignlStatus= true;
	}

};

class Soldier
{
public:
	void attack()
	{
		std::cout<<"soldier is attacking"<<std::endl;
	}

};
class FBI
{
	std::vector<Soldier> m_soldiers{};
	Signal m_sig;
	void soldiers_wait_for_signal()
	{
		std::cout<<"soldiers waiting for signal"<<std::endl;
		std::unique_lock<std::mutex> ul(m_sig.m_mtx);
		m_sig.m_cv.wait(ul,std::bind(&Signal::get_Signal_Status,&m_sig));
		std::cout<<"got signal"<<std::endl;
	}
	void attack()
	{
		for(Soldier& slr : m_soldiers)
		{
			slr.attack();
		}
	}
public:
	void add_soldiers(int num){
		for(int i=0; i<num; ++i)
		m_soldiers.push_back(Soldier());
	}
	void save_child()
	{
		std::cout<<"saving child operation started"<<std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(10));
		std::cout<<"saved child"<<std::endl;
		std::cout<<"turning the signal ON"<<std::endl;
		std::lock_guard<std::mutex> lg(m_sig.m_mtx);
		m_sig.blow();
		m_sig.m_cv.notify_all();

	}
	void get_soldiers_ready()
	{
		std::cout<<"getting soldiers ready"<<std::endl;
		soldiers_wait_for_signal();
		attack();
	}

};

int main()
{
	FBI obj;
	obj.add_soldiers(4);
	std::thread th1(&FBI::get_soldiers_ready,&obj);
	std::thread th2(&FBI::save_child,&obj);

	th1.join();
	th2.join();
	return 0;
}
