#include <iostream>// for io
#include <fstream> // to read the files

/*
  Response structure
  link : https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ata/ns-ata-_identify_device_data
*/ 
struct ResponseResult {
    char modelNumber[40]; // page 91 mention it as : Model number (40 ASCII characters)
    char Ulra_DMA_supportModel;    // support model will return in here- page 101
    bool smart_Self_TestSupported;  // is self test support or not  - page 94 input
};

/*
 Responislbe to handle request according to the file information
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

   // Task 01 - find Highest ultra DMA values part - start
    file.seekg(88, std::ios::beg); // according to page 96 - it mentioned binary data should read line - 88
    file.read(&response.Ulra_DMA_supportModel, 1); 
   // Task 01 - find Highest ultra DMA values part - end
    

    file.close();
    return response;
}

/**
 * Resposnible to return the highest ultra DMA values According to  ATA IDENTIFY response 
 * There are eight blocks and we have to read each box and (if the box enable) that will the hifhest mode (but should read 7 to 1)
 */
int get_Highest_Model(char Ulra_DMA_supportModel){
    // Find the highest supported Ultra-DMA mode
    int highestSupportedMode = -1;
    for (int i = 7; i >= 0; --i) {
        if (Ulra_DMA_supportModel & (1 << i)) {
            highestSupportedMode = i;
            break;
        }
    }
    return highestSupportedMode; // According to data set it will return any value in every time, then I avoid to handle errors in here
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
    std::cout << "Highest Supported Ultra-DMA Mode: " << get_Highest_Model(response.Ulra_DMA_supportModel) << std::endl;

    return EXIT_SUCCESS;
}

