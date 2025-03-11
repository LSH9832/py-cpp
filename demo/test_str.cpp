#ifndef TEST_STR_CPP
#define TEST_STR_CPP

#include "pylike/str.h"

int main()
{
    std::cout << "check split and convert pystring to int -----------" << std::endl;
    pystring location = "146,235,88,92";
    std::cout << location << std::endl;
    for (int number: location.split(","))   // double float string bool
    {
        std::cout << number << std::endl;
    }

    std::cout << "\ncheck split and convert pystring to bool ----------" << std::endl;
    pystring flags = "true;false;1;0";
    std::cout << flags << std::endl;
    for (bool flag: flags.split(";"))
    {
        std::cout << flag << std::endl;
    }

    std::cout << "\ncheck endwith something ---------------------------" << std::endl;
    pystring video_path = "1.mp4";
    if (video_path.endswith(".mp4"))
    {
        std::cout << "support format." << std::endl;
    }
    else
    {
        std::cout << video_path.split(".")[1] << " format is not supported now.";
    }

    std::cout << "\ncheck zero fill -----------------------------------" << std::endl;
    for (int i=0;i<20;i++)
    {
        std::cout << pystring(i).zfill(6) << std::endl;
    }

    std::cout << "\ncheck ljust rjust upper lower ----------------------" << std::endl;
    pystring person1 = "Li Hua", person2 = "Han Mei Mei";
    int person1_age = 25, person2_age = 20;
    std::cout << person1.upper().ljust(20) << ": " << person1_age << " years old" << std::endl;
    std::cout << person2.lower().rjust(20) << ": " << person2_age << " years old" << std::endl;

    std::cout << "\ncheck replace --------------------------------------" << std::endl;
    pystring message = "You do a very good job, Bob! Very good!";
    message = message.replace("good", "bad");

    std::cout << message << std::endl;


    return 0;
}

#endif