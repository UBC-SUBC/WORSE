#include <iostream> 
#include <vector> 
#include <fstream>
#include <string>

/*
* This code determines the change required in the centre of buoyancy based on the values of θ, θ', and θ"
* These values are inputted using a CSV
* */

using namespace std; 

int main(){

    //copy the values from the CSV to four different arraylists - one for each column

    vector<double> thetaList; 
    vector<double> deltaTList; 
    vector<double> accList; 
    vector<double> depthList; 

    double theta, deltaT, acceleration;
    string filePath = "C:\\Users\\cxson\\OneDrive\\Desktop\\Documents\\SUBC\\WORSE\\src\\realTest.csv";

    //variables for the purpose of testing verification - should change numTestCases for the current test
    int numTestCases = 17; 
    int numInstructions = 0; 

    //read from file
    fstream fileIn;
    fileIn.open(filePath, ios::in);
    string data;
    getline(fileIn,data); //gets the first line (i think)
    while(!fileIn.eof()){ //(kinda like hasNext() hopefully?)
        getline(fileIn,data); //gets values then splits it 
        string values[] = //need to split everything by comma
    }


    /*
			Scanner inputStream;
			{
				try {
					inputStream = new Scanner(file);
					String data = inputStream.next(); //ignore first line
					while(inputStream.hasNext()){
						data = inputStream.next(); //data is the entire line
						String[] values = data.split(",");
						thetaList.add(Double.parseDouble(values[0]));
						deltaTList.add(Double.parseDouble(values[1]));
						accList.add(Double.parseDouble(values[2]));
						depthList.add(Double.parseDouble(values[3])); 

						//System.out.println(values[3]); //this means get all the values in the 4th column
					}s
					inputStream.close();
				} catch (FileNotFoundException e) {
					e.printStackTrace();
				}
            }*/


    int depthInstruction = 0; 
    int finalInstruction = 0; 

    for(int i = 0; i < thetaList.size(); i++) {

        theta = thetaList.at(i);
        deltaT = deltaTList.at(i);
        acceleration = accList.at(i);

        int instruction = 0;
        int R = 0;
        int L = 1;

        /*
        encoding that gives the signs of the three parameters
        Add nothing if any of the parameters = 0
        1 = positive
        2 = negative
        */
        int code = 0;

        if (theta > 0.0)
            code += 100;
        else if (theta < 0.0)
            code += 200;

        if (deltaT > 0.0)
            code += 10;
        else if (deltaT < 0.0)
            code += 20;

        if (acceleration > 0.0)
            code += 1;
        else if (acceleration < 0.0)
            code += 2;

        switch (code) {

            //Case 7
            case 121:
                instruction = R * 10 + 1;
                break;
            
            //Case 19
            case 211:
                instruction = L * 10 + 1;
                break;

            //Case 3, 5, 10, 11 and 12 
            case 112: 
            case 100: 
            case 11: 
            case 10: 
            case 12: 
                instruction = L * 10 + 2; 
                break;

            //Case 16, 17, 18, 23, 27
            case 21:
            case 20:
            case 22:
            case 200:
            case 222:
                instruction = R * 10 + 2; 
                break;
                
            //Case 25 and 26
            case 221:
            case 220:
                instruction = R * 10 + 3;
                break;

            //Case 1 and 2 
            case 111: 
            case 110: 
                instruction = L * 10 + 3; 
                break; 
            
            //Case 14 
            case 0: 
                instruction = 0; 
                break; 
            
            //Observe further cases (ie. Case 8, 9, 20, 21) (the blue ones)
            default: 
                instruction = -1; 
        }
        numInstructions++; 
        //call the overallBuoyancy function 
        if(instruction != -1){
            depthInstruction = overallBuoyancy(depthList, i);

            //the first two digits of the final instruction represent the overall buoyancy instruction
            //The last two represent the CoB instruction 
            finalInstruction = depthInstruction*100 + instruction; 
            
            checkInstruction(finalInstruction, numInstructions);
        }
    }

    if (numTestCases +2 == numInstructions) //The +2 takes into account the two dummy cases at the end
        cout << "\nNumber of instructions is correct";
    else 
       cout << "\nError: missing " << (numTestCases - numInstructions) << " instructions";
}

static int overallBuoyancy (vector<double> depthList, int startIndex){
    int instruction = 0; 
    int encoding = 0;
    int positive = 1, increase = 1;
    int negative = 2, decrease = 2;		

    /*Set delta1 and delta2 
    delta1 = depth2 - depth1 
    delta2 = depth3 - depth2 */
    double delta1, delta2;

    if((startIndex+2) < depthList.size()){
        delta1 = depthList.at(startIndex + 1) - depthList.at(startIndex);
        delta2 = depthList.at(startIndex + 2) - depthList.at(startIndex + 1);

        /*
        * Encoding:
        *   The tens digit will represent the sign of delta1 and the ones digit the sign of delta 2
        */
        if(delta1 > 0)
            encoding += positive*10;
        else if(delta1 < 0)
            encoding += negative*10;

        if(delta2 > 0)
            encoding += positive;
        else if(delta2 < 0)
            encoding += negative;
        

        if(delta1>delta2){
            switch(encoding){
                case 11: 
                    instruction += decrease*10 + 1;
                    break;
                case 12: 
                    instruction += increase*10 + 1;
                    break;
                //Note: delta1>delta2 has a different meaning when it comes to negative numbers
                case 22:
                    instruction += increase*10 + 2;
                    break; 
                case 2:
                    instruction += increase*10 + 1;
                    break;
                    
                //case 21 is impossible
                //default includes case 10 where we want instruction to be 0
                default:
                    instruction = 0;
            }
        }

        if(delta2>delta1){
            switch(encoding){
                case 11:
                    instruction += decrease*10 + 2;
                    break;
                case 22:
                    instruction += increase*10 + 1;
                    break;
                case 21:
                    instruction += decrease*10 + 1;
                    break;
                case 1: 
                    instruction += decrease*10 + 1;
                    break;

                //case 12 is impossible
                //default includes case 20 where we want instruction to be 0
                default:
                    instruction = 0;
            }
        }

        if(delta1 == delta2){
            switch(encoding){
                case 11:
                    instruction += decrease*10 + 1;
                    break;
                case 22:
                    instruction += increase*10 + 1;
                    break;
                //default includes cases 11, 12 where we want instruction to be 0
                default:
                    instruction = 0;
            }
        }
    }
    //last two lines will be ignored
    else{
        cout << "\nReached max"; 
        instruction = -1; 
    }
    
    return instruction; 
}

static int checkInstruction(int instruction, int numInstructions){
			
    cout << "\nInstruction: " << instruction;
    if((numInstructions % 9) == 0){
        cout << "_________________________________"; 
    }
    return instruction;
}