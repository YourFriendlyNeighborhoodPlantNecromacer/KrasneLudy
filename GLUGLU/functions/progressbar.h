#ifndef __PROGRESSBAR_H__
#define __PROGRESSBAR_H__

double progressbar(double task_progress, double task_completion, const std::string task){
    std::cout << task << ": [";
    int64_t i=0;
    for(i=0; i<(int)(task_progress/task_completion*10); i++) std::cout << '#';//10
    for(;i<10;i++) std::cout << " ";
    std::cout << "]" << std::endl;
    return task_progress/task_completion;
}

#endif//__PROGRESSBAR_H__
