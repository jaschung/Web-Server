Compile:
g++ -std=c++11 two_helloworlds.cpp -o hello -lwthttp -lboost_system -lwt

Running:
./hello --docroot /usr/local/share/Wt --http-address 0.0.0.0 --http-port 8083

NOTE:
To see output for JSON stuff from "../people", "../people/1", "../people/2" ... etc,
view the "raw data" tab to see output if "JSON" tab gives an error.