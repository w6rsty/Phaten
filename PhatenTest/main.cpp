#include "Input/Application.hpp"

int main(int argc, char *argv[])
{
    auto app = Pt::CreateScoped<Pt::Application>();

    app->Run();

    return 0;
}