#include "work.h"

#include <chrono>
#include <exception>
#include <future>
#include <iostream>
#include <string>
#include <syncstream>
#include <thread>

using namespace std;
using namespace std::chrono;

static void compute(const string& name, int seconds)
{
    this_thread::sleep_for(chrono::seconds(seconds));
    osyncstream(cout) << name << '\n';
}

static void slow(const string& name) { compute(name, 7); }
static void quick(const string& name) { compute(name, 1); }

void work()
{
    const auto start = steady_clock::now();

    promise<void> a1_done;
    shared_future<void> a1_ready = a1_done.get_future().share();

    future<void> side = async(launch::async, [a1_ready]
    {
        quick("A2");
        quick("B2");
        quick("C2");
        a1_ready.get();
        quick("C1");
    });

    try
    {
        slow("A1");
        a1_done.set_value();
    }
    catch (...)
    {
        a1_done.set_exception(current_exception());
        throw;
    }

    slow("B1");

    side.get();
    quick("D");

    const duration<double> elapsed = steady_clock::now() - start;
    osyncstream(cout) << "Time: " << elapsed.count() << " s\n";
    osyncstream(cout) << "Work is done!\n";
}