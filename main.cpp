#include <iostream>// for io
#include <fstream> // to read the files

/*
  Response structure
  link : https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ata/ns-ata-_identify_device_data
*/ 
struct ResponseResult {
    char modelNumber[40]; // page 91 mention it as : Model number (40 ASCII characters)
    char UltraDMA_supportModel;    // support model will return in here- page 101
    bool smart_Self_TestSupported;  // is self test support or not  - page 94 input
};

/*
 
*/
ResponseResult commandRequest(const char* filename) {
    ResponseResult response;
    std::ifstream file(filename, std::ios::binary); // stream the file
    if (!file.is_open()) { // open the file 
        std::cerr << "Error opening file: " << filename << std::endl; // if can't open give an error 
        exit(EXIT_FAILURE); // retun with failor status 
    }

    // Task 01 - Model Number part - start
    // Move to the relevant position in the file
    file.seekg(27, std::ios::beg); // according to page 91 - it mentioned binary data should read line - 27 to 46 read  Model number and (40 ASCII characters)

    // Read the model number
    file.read(response.modelNumber, 40);
    response.modelNumber[40] = '\0'; // Null-terminate the model number as empty
    //  Task 01 - Model Number part - End


    file.close();
    return response;
}

int main(int argc, char* argv[]) {
    // Check for the correct number of command line arguments
    if (argc != 2) {
        std::cerr << "Reading : " << argv[0] << " : Fine-Name" << std::endl;
        return EXIT_FAILURE;
    }

    const char* filename = argv[1];
    ResponseResult response = commandRequest(filename);
    // Print the extracted information
    std::cout << "Model Number: " << response.modelNumber << std::endl;
    

    return EXIT_SUCCESS;
}