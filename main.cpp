#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include <algorithm>
using namespace std;

void PrintStack(stack<string> s,std::ofstream& myFile);

int main(int argc, char *argv[]) {
    string myText;
    ifstream dpda(argv[1]);
    ifstream input1(argv[2]);
    ofstream myFile(argv[3]);

    vector<string> the_inputs;
    vector<vector<string>> inputsx;
    vector<string> dpdas;
    stack<string> stack;

    while (getline (dpda, myText)) {
        dpdas.push_back(myText);
    }
    while (getline (input1, myText)) {
        the_inputs.push_back(myText);
    }
    for(int i=0;i<the_inputs.size();i++){
        vector<string> temp;
        stringstream s(the_inputs[i]);
        string substr;
        while ( getline( s, substr, ',' )){
            temp.push_back( substr );
        }
        inputsx.push_back(temp);
    }
    string first_line=dpdas[0];
    string first_state="";
    unsigned space=first_line.find_last_of(" ");
    string asd=first_line.substr(space+1);
    stringstream stringstream1(asd);
    vector<string> first_and_final_states;
    vector<string> final_states;
    while( stringstream1.good() )
    {
        string substr;
        getline( stringstream1, substr, ',' );
        first_and_final_states.push_back( substr );
    }
    for(int i=0;i<first_and_final_states.size();i++){
        if (first_and_final_states[i].find("(") != std::string::npos) {
            string temp = first_and_final_states[i].substr(1, first_and_final_states[i].size() - 2);
            first_state.append(temp);
        }else if(first_and_final_states[i].find("[") != std::string::npos){
            string temp = first_and_final_states[i].substr(1, first_and_final_states[i].size() - 2);
            final_states.push_back(temp);
        }
    }
    unsigned first=first_line.find(':');
    unsigned last=first_line.find('=');
    string states1= first_line.substr(first+1,last-3);
    string buf;
    stringstream ss(states1);
    vector<string> states;
    while( ss.good() )
    {
        string substr;
        getline( ss, substr, ',' );
        states.push_back( substr );
    }
    string x=dpdas[1].substr(first+1);
    stringstream sss(x);
    vector<string> input_alphabet;
    while( sss.good() )
    {
        string substr;
        getline( sss, substr, ',' );
        input_alphabet.push_back( substr );
    }
    string y=dpdas[2].substr(first+1);
    stringstream ssss(y);
    vector<string> stack_alphabet;
    while( ssss.good() )
    {
        string substr;
        getline( ssss, substr, ',' );
        stack_alphabet.push_back( substr );
    }
    bool is_dollar_sign=false;
    for(int i=0;i<stack_alphabet.size();i++){
        if(stack_alphabet[i]=="$"){
            is_dollar_sign= true;
        }
    }
    vector<vector<string>> transitions;
    for(int i=3;i<dpdas.size();i++){
        vector<string> inside_of_transitions;
        string t= dpdas[i].substr(first+1);
        stringstream stringstream2(t);
        while( stringstream2.good() )
        {
            string substr;
            getline( stringstream2, substr, ',' );
            inside_of_transitions.push_back( substr );
        }
        transitions.push_back(inside_of_transitions);
    }
    bool is_input_alphabet=true;
    for(size_t i=0;i<transitions.size();i++){
        string key=transitions[i][1];
        if (!(count(input_alphabet.begin(), input_alphabet.end(), key)||key=="e")) {
            is_input_alphabet= false;
        }
    }
    bool is_stack_pop= true;
    bool is_stack_push= true;
    for(int i=0;i<transitions.size();i++){
        string key=transitions[i][2];
        if (!(count(stack_alphabet.begin(), stack_alphabet.end(), key)||key=="e")) {
            is_stack_pop= false;
        }
    }
    for(int i=0;i<transitions.size();i++){
        string key=transitions[i][4];
        if (!(count(stack_alphabet.begin(), stack_alphabet.end(), key)||key=="e")) {
            is_stack_push= false;
        }
    }
    bool is_curr_state= true;
    bool is_next_state=true;
    for(int i=0;i<transitions.size();i++){
        string key=transitions[i][0];
        if (!count(states.begin(), states.end(), key)) {
            is_curr_state= false;
        }
    }
    for(int i=0;i<transitions.size();i++){
        string key=transitions[i][3];
        if (!count(states.begin(), states.end(), key)) {
            is_next_state= false;
        }
    }

//---------------------------------now it begins---------------------------------------

    bool is_first_state_a_last=false;
    for(int t=0;t<final_states.size();t++){
        const string& keey=first_state;
        if (count(final_states.begin(), final_states.end(), keey)) {
            is_first_state_a_last= true;
        }
    }
    string current_state=first_state;
    vector<string> inputs;
    if(is_curr_state && is_next_state && is_input_alphabet && is_stack_pop && is_stack_push){
        for(int x=0;x<inputsx.size();x++){
            inputs=inputsx[x];
            current_state=first_state;
            int i=0;
            bool is_empty_input=false;
            if (inputs.empty()){
                is_empty_input= true;
            }
            bool is_first_state_passed_successfully=false;
            bool is_final_state= false;
            bool is_accept=false;
            if(is_empty_input){
                if(is_first_state_a_last){
                    is_accept=true;
                    myFile<<"ACCEPT"<<endl;
                    myFile<<endl;
                    continue;
                }else{
                    is_accept=false;
                    myFile<<"REJECT"<<endl;
                    myFile<<endl;
                    continue;
                }

            }
            while (i<inputs.size()){
                bool is_e_necessary=true;
                bool no_more_match=false;
                for(int t=0;t<transitions.size();t++){
                    if(stack.empty()){
                        if(transitions[t][0]==current_state && transitions[t][1]==inputs[i]&&transitions[t][2]=="e" ){
                            is_e_necessary= false;
                        }
                        if(transitions[t][0]==current_state && (transitions[t][1]==inputs[i] || transitions [t][1]=="e")&&transitions[t][2]=="e"){
                            no_more_match= true;
                        }
                    }else{
                        if(transitions[t][0]==current_state && transitions[t][1]==inputs[i]&&(transitions[t][2]==stack.top()||transitions[t][2]=="e") ){
                            is_e_necessary= false;
                        }
                        if(transitions[t][0]==current_state && (transitions[t][1]==inputs[i] || transitions [t][1]=="e")&&(transitions[t][2]==stack.top()||transitions[t][2]=="e")){
                            no_more_match= true;
                        }
                    }
                }
                if(!no_more_match){
                    break;
                }
                string z= inputs[i];

                if(!is_e_necessary){
                    for (int j=0;j<transitions.size();j++){
                        if(i==inputs.size()){
                            break;
                        }
                        if(transitions[j][0]==current_state){
                            if( current_state==first_state && transitions[j][1]==inputs[i] && transitions[j][2]=="e") {
                                current_state = transitions[j][3];
                                if(transitions[j][4]!="e"){
                                    stack.push(transitions[j][4]);
                                    myFile<<first_state<<","<<inputs[i]<<","<<"e"<<" => "<<current_state<<","<<transitions[j][4]<<" [STACK]:";
                                    PrintStack(stack,myFile);
                                    myFile<<endl;
                                }else{
                                    myFile<<first_state<<","<<inputs[i]<<","<<"e"<<" => "<<current_state<<","<<"e"<<" [STACK]:";
                                    PrintStack(stack,myFile);
                                    myFile<<endl;
                                }
                                i++;
                                break;
                            }else{
                                //-------------------------------------------------------------------------------
                                if(transitions[j][1]==inputs[i] && (transitions[j][2]=="e"||transitions [j][2]==stack.top())){
                                    if(transitions[j][2]=="e"){
                                        current_state=transitions[j][3];
                                        if(transitions[j][4] != "e"){
                                            stack.push(transitions[j][4]);
                                            myFile<<transitions[j][0]<<","<<inputs[i]<<","<<"e"<<" => "<<current_state<<","<<transitions[j][4]<<" [STACK]:";
                                            PrintStack(stack,myFile);
                                            myFile<<endl;
                                        }else{
                                            myFile<<transitions[j][0]<<","<<inputs[i]<<","<<"e"<<" => "<<current_state<<","<<"e"<<" [STACK]:";
                                            PrintStack(stack,myFile);
                                            myFile<<endl;
                                        }
                                        i++;
                                        break;
                                    } else{
                                        current_state = transitions[j][3];
                                        stack.pop();
                                        if (transitions[j][4] != "e") {
                                            stack.push(transitions[j][4]);
                                            myFile<<transitions[j][0]<<","<<inputs[i]<<","<<transitions[j][2]<<" => "<<current_state<<","<<transitions[j][4]<<" [STACK]:";
                                            PrintStack(stack,myFile);
                                            myFile<<endl;
                                        }else{
                                            myFile<<transitions[j][0]<<","<<inputs[i]<<","<<transitions[j][2]<<" => "<<current_state<<","<<"e"<<" [STACK]:";
                                            PrintStack(stack,myFile);
                                            myFile<<endl;
                                        }
                                        i++;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }else{
                    for (int j=0;j<transitions.size();j++){
                        if(i==inputs.size()){
                            break;
                        }
                        if(transitions[j][0]==current_state){
                            if(current_state==first_state &&transitions[j][1]=="e" && transitions[j][2]=="e"){

                                current_state=transitions[j][3];
                                if(transitions[j][4]!="e"){
                                    stack.push(transitions[j][4]);
                                    myFile<<first_state<<","<<"e"<<","<<"e"<<" => "<<current_state<<","<<transitions[j][4]<<" [STACK]:";
                                    PrintStack(stack,myFile);
                                    myFile<<endl;
                                }else{
                                    myFile<<first_state<<","<<"e"<<","<<"e"<<" => "<<current_state<<","<<"e"<<" [STACK]:";
                                    PrintStack(stack,myFile);
                                    myFile<<endl;
                                }
                                is_first_state_passed_successfully= true;
                                break;
                            }else if (transitions[j][1]=="e" && (transitions[j][2]=="e"||transitions [j][2]==stack.top())){
                                if(transitions[j][2]=="e"){
                                    current_state=transitions[j][3];
                                    if(transitions[j][4] != "e"){
                                        stack.push(transitions[j][4]);
                                        myFile<<transitions[j][0]<<","<<"e"<<","<<"e"<<" => "<<current_state<<","<<transitions[j][4]<<" [STACK]:";
                                        PrintStack(stack,myFile);
                                        myFile<<endl;
                                    } else{
                                        myFile<<transitions[j][0]<<","<<"e"<<","<<"e"<<" => "<<current_state<<","<<"e"<<" [STACK]:";
                                        PrintStack(stack,myFile);
                                        myFile<<endl;
                                    }
                                    break;
                                } else{
                                    current_state = transitions[j][3];
                                    stack.pop();
                                    if (transitions[j][4] != "e") {
                                        stack.push(transitions[j][4]);
                                        myFile<<transitions[j][0]<<","<<"e"<<","<<transitions[j][2]<<" => "<<current_state<<","<<transitions[j][4]<<" [STACK]:";
                                        PrintStack(stack,myFile);
                                        myFile<<endl;
                                    } else{
                                        myFile<<transitions[j][0]<<","<<"e"<<","<<transitions[j][2]<<" => "<<current_state<<","<<"e"<<" [STACK]:";
                                        PrintStack(stack,myFile);
                                        myFile<<endl;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            for(int p=0;p<final_states.size();p++){
                if(final_states[p]==current_state){
                    is_final_state= true;
                }
            }
            if(!is_final_state){
                for (int j=0;j<transitions.size();j++){
                    if(stack.empty()){
                        if(current_state==transitions[j][0] && transitions[j][1]=="e" && transitions[j][2]=="e"){
                            current_state=transitions[j][3];
                            if(transitions[j][4]!="e"){
                                stack.push(transitions[j][4]);
                                myFile<<transitions[j][0]<<","<<"e"<<","<<"e"<<" => "<<current_state<<","<<transitions[j][4]<<" [STACK]:";
                                PrintStack(stack,myFile);
                                myFile<<endl;
                                break;
                            } else{
                                myFile<<transitions[j][0]<<","<<"e"<<","<<"e"<<" => "<<current_state<<","<<"e"<<" [STACK]:";
                                PrintStack(stack,myFile);
                                myFile<<endl;
                                break;
                            }
                        }
                    }else{
                        if(current_state==transitions[j][0] && transitions[j][1]=="e"){
                            if(transitions[j][2]=="e"){
                                current_state=transitions[j][3];

                                if(transitions[j][4]!="e"){
                                    stack.push(transitions[j][4]);
                                    myFile<<transitions[j][0]<<","<<"e"<<","<<"e"<<" => "<<current_state<<","<<transitions[j][4]<<" [STACK]:";
                                    PrintStack(stack,myFile);
                                    myFile<<endl;
                                    break;
                                } else{
                                    myFile<<transitions[j][0]<<","<<"e"<<","<<"e"<<" => "<<current_state<<","<<"e"<<" [STACK]:";
                                    PrintStack(stack,myFile);
                                    myFile<<endl;
                                    break;
                                }
                            }else{
                                current_state=transitions[j][3];
                                stack.pop();
                                if(transitions[j][4]!="e"){
                                    stack.push(transitions[j][4]);
                                    myFile<<transitions[j][0]<<","<<"e"<<","<<transitions[j][2]<<" => "<<current_state<<","<<transitions[j][4]<<" [STACK]:";
                                    PrintStack(stack,myFile);
                                    myFile<<endl;
                                    break;
                                } else{
                                    myFile<<transitions[j][0]<<","<<"e"<<","<<transitions[j][2]<<" => "<<current_state<<","<<"e"<<" [STACK]:";
                                    PrintStack(stack,myFile);
                                    myFile<<endl;
                                    break;
                                }
                            }
                        }

                    }
                }
            }

            for(int p=0;p<final_states.size();p++){
                if(final_states[p]==current_state){
                    is_final_state= true;
                }
            }
            if(is_dollar_sign){
                if(is_final_state && (stack.empty()||(stack.size()==1 && stack.top()=="$"))){
                    is_accept=true;
                }
            }else{
                if(is_final_state && stack.empty()){
                    is_accept=true;
                }
            }
            if(is_accept){
                myFile<<"ACCEPT"<<endl;
                myFile<<endl;
            } else{
                myFile<<"REJECT"<<endl;
                myFile<<endl;
            }



            while (!stack.empty()){
                stack.pop();
            }
        }
    }else{
        myFile<<"Error [1]:DPDA description is invalid!";
    }
    myFile.close();
    input1.close();
    dpda.close();
    return 0;
}



void PrintStack(stack<string> s,std::ofstream& myFile)
{
    stack<string> temp;
    while (s.empty() == false)
    {
        temp.push(s.top());
        s.pop();
    }

    while (temp.empty() == false)
    {
        string t = temp.top();
        myFile << t ;
        if(temp.size()>1){
            myFile<< ",";
        }
        temp.pop();

        s.push(t);
    }
}