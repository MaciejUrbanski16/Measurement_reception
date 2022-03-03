// wxWidgets "Hello World" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include "boost/asio.hpp"
#include <thread>
//#include <boost/thread.hpp>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP

#include <wx/dc.h>
#include <wx/gdicmn.h>

#endif

#include "RemoteDataInterpreter.h"
#include "PseudoServerContext.h"

class ReceivedDataContainer
{

};
class RemoteDataInterpreter;

class PseudoServerTracker : public wxApp
{
public:
    virtual bool OnInit();
};

bool PseudoServerTracker::OnInit()
{

    RemoteDataInterpreter *frame = new RemoteDataInterpreter();
    frame->Show();



//    boost::asio::io_context io_context;
//    std::shared_ptr<boost::asio::io_context::work> work(new boost::asio::io_context::work(io_context));
////    std::thread server([&](){
////
////
////        PseudoServerContext pseudoServerContext(io_context, 8000);
////        pseudoServerContext.async_accept();
////        io_context.run();
////    }
////        );
//    auto a1 = std::async([&io_context](){});
//   // work.reset();
//
//   // boost::thread t;//(&boost::asio::io_context::run, &io_context);
//
//    std::cout<<"window join\n";
//    //server.join();
//    std::cout<<"server join\n";


    //server.join();

    return true;
}


wxIMPLEMENT_APP(PseudoServerTracker);




