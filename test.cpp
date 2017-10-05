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

#include <string>
#include "zz.cpp"


using namespace Wt;

class HelloApplication : public WApplication
{
public:
	HelloApplication(const WEnvironment& env);

private:
	WLineEdit *nameEdit_;
	WText *greeting_;
	WPanel *panel_;
	bool on;
	void greet();
};


HelloApplication::HelloApplication(const WEnvironment& env)
	: WApplication(env)
{
	setTitle("Welcome to Person Generator");                               // application title

	root()->addWidget(new WText("Enter a number to generate number of people: "));  // show some text
	
	

	nameEdit_ = new WLineEdit(root());                     // allow text input
	nameEdit_->setFocus();                                 // give focus

	WPushButton *button
		= new WPushButton("Generate", root());              // create a button
	button->setMargin(5, Left);                            // add 5 pixels margin

	root()->addWidget(new WBreak());                       // insert a line break

	greeting_ = new WText(root());

	on = false;

	button->clicked().connect(this, &HelloApplication::greet);

	nameEdit_->enterPressed().connect
	(boost::bind(&HelloApplication::greet, this));


}



void HelloApplication::greet()
{

	WString num = nameEdit_->text();
	std::wstring wide = num.value();
	int number = std::stoi(wide);
	

	for (int i = 0; i < number; i++)
	{
		panel_ = new WPanel(root());
		
		string index = to_string(i+1);
		Randomizer jason;
		jason.makePerson();
		root()->addWidget(new WBreak());
		panel_->setTitle("Person " + index);
		panel_->setCentralWidget(greeting_);
		greeting_->setText(jason.personToString());
		greeting_ = new WText(root());

	}

}


WApplication *createApplication(const WEnvironment& env)
{
	return new HelloApplication(env);
}



int main(int argc, char **argv)
{
	srand(time(NULL));

	return WRun(argc, argv, &createApplication);
}