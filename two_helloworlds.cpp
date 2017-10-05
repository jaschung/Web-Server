#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WException>
#include <Wt/WLogger>
#include <Wt/WServer>
#include <Wt/WPanel>

#include <Wt/Http/Response>

#include <boost/thread.hpp>

#include <Wt/WResource>
#include <vector>

#include <string>
#include "zz.cpp"
using namespace Wt;
using namespace std;

//Vector holding Randomizers, which holds each random person
vector<Randomizer> PersonVector;

class MyResource : public Wt::WResource
{
public:
	MyResource()
		: WResource()
	{
		
	}
	void handleRequest(const Wt::Http::Request &request,
		Wt::Http::Response &response) {
		
		response.setMimeType("application/json");
		ostream& out = response.out();

		//output JSON if a specific index is specified in URL for person
		if (request.pathInfo() != "")
		{
			string personIndex = (request.pathInfo().substr(request.pathInfo().find("/") + 1));
			int pos = stoi(personIndex);
			out << "{" << endl;
			out << "\t ID: " << pos << endl;
			out << "\t Name: " << PersonVector.at(pos - 1).getPerson().name.getName() << endl;
			out << "\t Address: " << PersonVector.at(pos - 1).getPerson().address.getAddress() << endl;
			out << "\t Phone: " << PersonVector.at(pos - 1).getPerson().phone.getNumber() << endl;
			out << "}" << endl;
		}

		//output JSON if no specific person is requested
		else
		{
			out << "{" << endl;
			out << "\t \"people\": [" << endl;
			for (int i = 0; i < PersonVector.size()-1; i++)
			{
				out << "\t\t { id "<< i+1 <<": " << PersonVector.at(i).personToJson() << " }," << endl;
			}
			out << "\t\t { id " << PersonVector.size() << ": " << PersonVector.at(PersonVector.size()-1).personToJson() << " }" << endl;
			out << "\t ]" << endl;
			out << "}" << endl;
		}
	}
};

class HelloApplication : public WApplication
{
public:
    HelloApplication(const WEnvironment& env, const std::string& title);

private:
    WLineEdit *nameEdit_;
    WText *greeting_;
	WPanel *panel_;
    void greet();

};

HelloApplication::HelloApplication(const Wt::WEnvironment& env, const std::string& title)
    : WApplication(env)
{
    setTitle("People Maker");

    root()->addWidget(new Wt::WText("Enter a number to generate this many people: "));
    nameEdit_ = new WLineEdit(root());
    WPushButton *button = new WPushButton("Generate", root());
    root()->addWidget(new WBreak());
    greeting_ = new WText(root());
    button->clicked().connect(this, &HelloApplication::greet);
}

void HelloApplication::greet()
{    
	//gets the user's number input and translates it into int
	WString num = nameEdit_->text();
	std::wstring wide = num.value();
	int number = std::stoi(wide);

	//refreshes the vector every times the button is pressed to remove previous data
	string display = "";
	PersonVector.clear();

	for (int i = 0; i < number; i++)
	{
		root()->addWidget(new WBreak());
		//makes a random person for every number entered
		string index = to_string(i + 1);
		Randomizer jason;
		jason.makePerson();
		jason.setID(index);
		//pushes the newly made person into vector
		PersonVector.push_back(jason);
		//creates output all into one formatted string similar to the Hello World example.
		display = display.append("<br/>" + PersonVector.at(i).personToString() + "<br/>");
		root()->addWidget(new WBreak());

	}

	//outputs all people into textbox
	greeting_->setText(display);

}


Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    return new HelloApplication(env, "People Generator");
}



int YourWRun(int argc, char *argv[], Wt::ApplicationCreator createApplication)
{
  try {
    // use argv[0] as the application name to match a suitable entry
    // in the Wt configuration file, and use the default configuration
    // file (which defaults to /etc/wt/wt_config.xml unless the environment
    // variable WT_CONFIG_XML is set)
    Wt::WServer server(argv[0],"");

    // WTHTTP_CONFIGURATION is e.g. "/etc/wt/wthttpd"
    server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

	MyResource dr;
	server.addResource(&dr, "/people");

    // add a single entry point, at the default location (as determined
    // by the server configuration's deploy-path)
    server.addEntryPoint(Wt::Application, createApplication);
    //server.addEntryPoint(Wt::Application, createSecondApplication,"/people");
    if (server.start()) {
      int sig = Wt::WServer::waitForShutdown(argv[0]);

      std::cerr << "Shutdown (signal = " << sig << ")" << std::endl;
      server.stop();

      /*
      if (sig == SIGHUP)
        WServer::restart(argc, argv, environ);
      */
      }
  } catch (Wt::WServer::Exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  } catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
    return 1;
  }
}


int main(int argc, char **argv)
{
	//for the rand function
	srand(time(NULL));
    return YourWRun(argc, argv, &createApplication);
}