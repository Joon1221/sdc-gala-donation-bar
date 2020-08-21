================================================================================
                             SDC Gala Donation Bar                                                       
================================================================================


INTRODUCTION
------------

Social Diversity for Children Foundation (SDC) is an organization that focuses
on helping children with disabilities by raising awareness, providing services,
and fundraising. This program was created for usage in SDC's 10th Anniversary
Gala as a donation bar that keeps track of the total amount of funds raised
from the gala. The gala reached its goal by raising $217,000 in donations.


STAFF
-----

Programmer : Joon Kang
Artist : Juan Juan Yin


WORK PERIOD
-----------

Start Date: 2019-12-26 
End Date: 2019-12-30


INSTALLATION
------------

- Install Homebrew
	- Open terminal and paste the following
	- /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/
	  install/master/install.sh)"
	- brew update
- Install Python 2 and Python 3
	- Open terminal and paste the following
	- brew install python python3
	- brew link python
	- brew link python3
- Update ~.bash_profile
	-nano ~/.bash_profile
		- Add the following and save .bash_profile
		- # Homebrew
		- export PATH=/usr/local/bin:$PATH
		- # point to python2
		- export PATH="/usr/local/opt/python/libexec/bin:$PATH"
	- source ~/.bash_profile
- Install OpenCV
	- Open terminal and paste the following
	- brew install opencv


HOW TO USE
----------

- Build and Run the program using Xcode
- Every time the space bar is pressed, the donation bar goes up by $500

LIBRARIES
---------

- OpenCV 4.0.1
