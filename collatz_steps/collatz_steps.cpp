#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
int64_t powInt64(int64_t base, int64_t exponent){
    int64_t result = 1;
    for(int64_t a=0; a < exponent; a++){
        result *= base;}
    return result;}
int genCombos(int steps, int terms){
    int stepLimit = steps-2-terms;
    uint64_t newCombo, read;
    if(stepLimit < terms){
        std::filesystem::remove("H:/combo"+std::to_string(terms-1)+".txt");
        return 0;}
    std::ofstream fileOut("H:/combo"+std::to_string(terms)+".txt", std::ios::binary);
    if(terms == 1){
        for(uint64_t a=0; a < stepLimit; a++){
            newCombo = (uint64_t)1 << a;
            fileOut.write(reinterpret_cast<char*>(&newCombo), sizeof newCombo);}}
    else{
        std::ifstream fileIn("H:/combo"+std::to_string(terms-1)+".txt", std::ios::binary);
        std::vector <int> prevCombo;
        fileIn.read(reinterpret_cast<char*>(&read), sizeof read);
        while(!fileIn.eof()){
            for(int b=0; b < 64; b++){
                if(read & powInt64(2, b)){
                    prevCombo.push_back(b+1);}}
            for(int b=prevCombo.back(); b < stepLimit; b++){
                newCombo = read + powInt64(2, b);
                fileOut.write(reinterpret_cast<char*>(&newCombo), sizeof newCombo);}
            prevCombo.clear();
            fileIn.read(reinterpret_cast<char*>(&read), sizeof read);}
        fileIn.close();}
    fileOut.close();
    if(terms > 1){
        std::filesystem::remove("H:/combo"+std::to_string(terms-1)+".txt");}
    std::ifstream fileIn("H:/combo"+std::to_string(terms)+".txt", std::ios::binary);
    std::string comboLine = "";
    std::vector <int> combo;
    int64_t denom = powInt64(2, (steps-(terms+1)))-powInt64(3, (terms+1));
    fileIn.read(reinterpret_cast<char*>(&read), sizeof read);
    while(!fileIn.eof()){
        int term = 1;
        for(int b=0; b < stepLimit; b++){
            if((read & powInt64(2, b)) > 0){
                combo.push_back(b+1);}}
        for(auto b: combo){
            if(b == combo.front()){
                comboLine = std::to_string(b);}
            else{
                comboLine += ","+std::to_string(b);}}
        int64_t sum = powInt64(3, terms);
        for(int b=0; b < combo.size(); b++){
            sum += powInt64(3, (terms-1-b))*powInt64(2, combo[b]);}
        if((sum%denom) == 0){
            std::ofstream fileOut("H:/results.txt", std::ios::app);
            fileOut << std::to_string(steps)+"|"+std::to_string(sum/denom)+"|"+std::to_string(sum)+"|"+std::to_string(denom)+"|"+comboLine+"\n";
            fileOut.close();}
        combo.clear();
        fileIn.read(reinterpret_cast<char*>(&read), sizeof read);}
    fileIn.close();
    return 1;}
int main(){
    std::vector <int> steps, sums, denoms;
    std::vector <std::vector <int>> combos;
    std::cout << "\n";
    int numSteps = 50;
    for(int step=41; step < (numSteps+1); step++){
        int genSuccess = 1, term = 1;
        std::string stepNum = std::to_string(step);
        stepNum.insert(stepNum.begin(), 3-stepNum.length(), ' ');
        std::cout << "\r    Calculating Step "+stepNum+"/"+std::to_string(numSteps)+"...\n";
        while(genSuccess){
            std::cout << "\r        Number of Terms: "+std::to_string(term);
            genSuccess = genCombos(step, term);
            term++;}}
    std::cout << "\r        Done!                   \n\n";
    std::ifstream fileIn("H:/results.txt");
    std::istringstream resultStream, comboStream;
    std::vector <std::string> results, combo;
    std::string result = "";
    int lastStepNum = 0;
    while(!fileIn.eof()){
        std::getline(fileIn, result);
        resultStream.str(result);
        for(std::string part; std::getline(resultStream, part, '|');){
            results.push_back(part);}
        if(results.size() < 5){
            break;}
        results[0].insert(results[0].begin(), 3-results[0].length(), ' ');  //step number
        results[1].insert(results[1].begin(), 5-results[1].length(), ' ');  //loop value
        results[2].insert(results[2].begin(), 20-results[2].length(), ' '); //numerator
        results[3].insert(results[3].begin(), 20-results[3].length(), ' '); //denominator
        comboStream.str(results[4]);
        for(std::string comboNum; std::getline(comboStream, comboNum, ',');){
            combo.push_back(comboNum);}
        results[4] = "<";
        for(auto b : combo){
            if(b != combo.back()){
                results[4] += b+", ";}
            else{
                results[4] += b+">\n";}}
        if(lastStepNum != std::stoi(results[0])){
            std::cout << "\n";}
        lastStepNum = std::stoi(results[0]);
        std::cout << results[0]+": "+results[1]+" | "+results[2]+" / "+results[3]+" | "+results[4];
        resultStream.clear();
        comboStream.clear();
        results.clear();
        combo.clear();}
    std::cout << "\n";
    system("pause");}
/*#include <numeric>
uint64_t choose(int set, int choice){ //67 choose 33 max
    uint64_t ans = 1;
    std::vector <int> factors(choice);
    for(int a=0; a < factors.size(); a++){
        if(((set-a)%choice) == 0){
            factors[a] = (set-a)/choice;
            choice--;}
        else{
            factors[a] = set-a;}}
    for(int a=0; a < (choice-1); a++){
        int divisor = choice-a;
        for(int b=0; b < factors.size(); b++){
            if(std::gcd(factors[b], divisor) > 1){
                int common = std::gcd(factors[b], divisor);
                factors[b] /= common;
                divisor /= common;
                if(divisor == 1){
                    std::erase_if(factors, [](int x){return x == 1;});
                    break;}}}}
    for(int a=0; a < factors.size(); a++){
        ans *= factors[a];}
    return ans;}*/