//
//  DesignOfMM.cpp
//  Assignment 3
//
//  Created by Alexie Gonzalez on 12/15/24.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <map>
#include <numeric>

using namespace std;

// Process class
class Process {
public:
    int pid;
    int arrival_time;
    int lifetime;
    vector<int> memory_pieces;
    int total_memory;
    int start_time;
    int completion_time;

    Process(int id, int arrival, int life, vector<int> memory)
        : pid(id), arrival_time(arrival), lifetime(life), memory_pieces(memory) {
        total_memory = accumulate(memory_pieces.begin(), memory_pieces.end(), 0);
        start_time = -1;
        completion_time = -1;
    }
};

// Function to load workload file
vector<Process> load_workload_file(const string &filename) {
    ifstream file(filename);
    vector<Process> processes;

    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        exit(1);
    }

    string line;
    getline(file, line);
    int num_processes = stoi(line);

    while (getline(file, line)) {
        if (line.empty()) continue;

        int pid = stoi(line);
        getline(file, line);
        istringstream arrival_stream(line);
        int arrival_time, lifetime;
        arrival_stream >> arrival_time >> lifetime;

        getline(file, line);
        istringstream memory_stream(line);
        vector<int> memory_pieces;
        string temp;
        memory_stream >> temp; // Skip the first piece (label)
        int mem;
        while (memory_stream >> mem) {
            memory_pieces.push_back(mem);
        }

        processes.emplace_back(pid, arrival_time, lifetime, memory_pieces);
    }
    file.close();
    return processes;
}

// Simulation function
void simulate(int memory_size, int page_size, vector<Process> &processes) {
    int num_frames = memory_size / page_size;
    vector<pair<int, int>> memory_map(num_frames, { -1, -1 }); // (pid, completion_time)
    queue<Process> input_queue;
    vector<string> events;
    int clock = 0;
    vector<int> turnaround_times;

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival_time < b.arrival_time;
    });

    while (!processes.empty() || !input_queue.empty() || any_of(memory_map.begin(), memory_map.end(),
                [](const pair<int, int> &p) { return p.first != -1; })) {
        // Add processes to input queue if they arrive at the current clock time
        while (!processes.empty() && processes.front().arrival_time <= clock) {
            events.push_back("Time " + to_string(clock) + ": Process " + to_string(processes.front().pid) + " arrives");
            input_queue.push(processes.front());
            processes.erase(processes.begin());
        }

        // Free memory for completed processes
        for (auto &frame : memory_map) {
            if (frame.first != -1 && frame.second <= clock) {
                events.push_back("Time " + to_string(clock) + ": Process " + to_string(frame.first) + " completes");
                frame = { -1, -1 }; // Free the memory frame
                turnaround_times.push_back(clock);
            }
        }

        // Attempt to admit processes from the input queue
        queue<Process> temp_queue;
        while (!input_queue.empty()) {
            Process process = input_queue.front();
            input_queue.pop();
            int required_frames = (process.total_memory + page_size - 1) / page_size;

            vector<int> free_frames;
            for (int i = 0; i < num_frames; ++i) {
                if (memory_map[i].first == -1) {
                    free_frames.push_back(i);
                }
            }

            if (free_frames.size() >= required_frames) {
                // Allocate memory
                for (int i = 0; i < required_frames; ++i) {
                    memory_map[free_frames[i]] = { process.pid, clock + process.lifetime };
                }
                process.start_time = clock;
                process.completion_time = clock + process.lifetime;
                events.push_back("Time " + to_string(clock) + ": Process " + to_string(process.pid) + " admitted to memory");
            } else {
                temp_queue.push(process); // Cannot admit, push back to queue
            }
        }
        input_queue = temp_queue;
        clock++;
    }

    // Calculate average turnaround time
    double avg_turnaround = accumulate(turnaround_times.begin(), turnaround_times.end(), 0.0) / turnaround_times.size();

    // Print events
    for (const auto &event : events) {
        cout << event << endl;
    }
    cout << "Average Turnaround Time: " << avg_turnaround << " ms" << endl;
}

int main() {
    // User inputs
    int memory_size;
    cout << "Memory size (e.g., 2000): ";
    cin >> memory_size;

    cout << "Page Size (1: 100, 2: 200, 3: 400): ";
    int page_size_option;
    cin >> page_size_option;

    map<int, int> page_size_map = { {1, 100}, {2, 200}, {3, 400} };
    int page_size = page_size_map[page_size_option];

    // Load processes from file
    string filename;
    cout << "Enter the workload file name (e.g., in1): ";
    cin >> filename;

    vector<Process> processes = load_workload_file(filename);

    // Run simulation
    simulate(memory_size, page_size, processes);

    return 0;
}
