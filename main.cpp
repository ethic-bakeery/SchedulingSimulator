#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    int waiting_time; // Added to store waiting time
};

bool compareArrivalTime(const Process& p1, const Process& p2) {
    return p1.arrival_time < p2.arrival_time;
}

bool compareBurstTime(const Process& p1, const Process& p2) {
    return p1.burst_time < p2.burst_time;
}

bool comparePriority(const Process& p1, const Process& p2) {
    return p1.priority < p2.priority;
}

void calculateFCFS(vector<Process>& processes) {
    int n = processes.size();
    int waiting_time = 0;
    int turnaround_time = 0;

    cout << "FCFS Scheduling:\n";
    cout << "Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";

    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time > turnaround_time) {
            turnaround_time = processes[i].arrival_time;
        }

        processes[i].waiting_time = turnaround_time - processes[i].arrival_time;
        waiting_time += processes[i].waiting_time; // Accumulate waiting time
        turnaround_time += processes[i].burst_time;

        cout << processes[i].id << "\t" << processes[i].arrival_time << "\t\t" << processes[i].burst_time << "\t\t" << processes[i].waiting_time << "\t\t" << turnaround_time << endl;
    }
}

void calculateSJF(vector<Process>& processes) {
    int n = processes.size();
    int waiting_time = 0;
    int turnaround_time = 0;

    sort(processes.begin(), processes.end(), compareBurstTime);

    cout << "\nShortest Job First (SJF) Scheduling:\n";
    cout << "Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";

    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time > turnaround_time) {
            turnaround_time = processes[i].arrival_time;
        }

        processes[i].waiting_time = turnaround_time - processes[i].arrival_time;
        waiting_time += processes[i].waiting_time; // Accumulate waiting time
        turnaround_time += processes[i].burst_time;

        cout << processes[i].id << "\t" << processes[i].arrival_time << "\t\t" << processes[i].burst_time << "\t\t" << processes[i].waiting_time << "\t\t" << turnaround_time << endl;
    }
}

void calculateRR(vector<Process>& processes, int quantum) {
    int n = processes.size();
    int remaining_time[n];
    int waiting_time = 0;
    int turnaround_time = 0;

    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
    }

    cout << "\nRound Robin Scheduling (Quantum=" << quantum << "):\n";
    cout << "Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";

    int time = 0;
    while (true) {
        bool done = true;
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0) {
                done = false;

                if (remaining_time[i] > quantum) {
                    time += quantum;
                    remaining_time[i] -= quantum;
                } else {
                    time += remaining_time[i];
                    processes[i].waiting_time += time - processes[i].arrival_time - processes[i].burst_time;
                    waiting_time += processes[i].waiting_time; // Accumulate waiting time
                    remaining_time[i] = 0;
                    turnaround_time += time - processes[i].arrival_time;
                }
            }
        }

        if (done) {
            break;
        }
    }

    for (int i = 0; i < n; i++) {
        cout << processes[i].id << "\t" << processes[i].arrival_time << "\t\t" << processes[i].burst_time << "\t\t" << processes[i].waiting_time << "\t\t" << turnaround_time << endl;
    }
}

void calculateSRTN(vector<Process>& processes) {
    int n = processes.size();
    int time = 0;
    int completed = 0;

    cout << "\nShortest Remaining Time Next (SRTN) Scheduling:\n";
    cout << "Time\tProcess\tArrival Time\tRemaining Time\n";

    while (completed < n) {
        int shortest = -1;
        int min_remaining_time = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && processes[i].remaining_time < min_remaining_time && processes[i].remaining_time > 0) {
                shortest = i;
                min_remaining_time = processes[i].remaining_time;
            }
        }

        if (shortest == -1) {
            time++;
            continue;
        }

        processes[shortest].remaining_time--;
        time++;

        cout << time << "\t" << processes[shortest].id << "\t" << processes[shortest].arrival_time << "\t\t" << processes[shortest].remaining_time << endl;

        if (processes[shortest].remaining_time == 0) {
            completed++;
            int waiting_time = time - processes[shortest].arrival_time - processes[shortest].burst_time;
            processes[shortest].waiting_time += waiting_time; // Accumulate waiting time
            cout << "Process " << processes[shortest].id << " completed. Waiting Time: " << waiting_time << endl;
        }
    }
}

void calculatePriority(vector<Process>& processes) {
    int n = processes.size();
    int waiting_time = 0;
    int turnaround_time = 0;

    sort(processes.begin(), processes.end(), comparePriority);

    cout << "\nPriority Scheduling:\n";
    cout << "Process\tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n";

    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time > turnaround_time) {
            turnaround_time = processes[i].arrival_time;
        }

        processes[i].waiting_time = turnaround_time - processes[i].arrival_time;
        waiting_time += processes[i].waiting_time; // Accumulate waiting time
        turnaround_time += processes[i].burst_time;

        cout << processes[i].id << "\t" << processes[i].arrival_time << "\t\t" << processes[i].burst_time << "\t\t" << processes[i].priority << "\t\t" << processes[i].waiting_time << "\t\t" << turnaround_time << endl;
    }
}

int main() {
    int n;
    int quantum;
    int choice;

    cout << "Choose a scheduling algorithm:\n";
    cout << "1. FCFS\n";
    cout << "2. SJF\n";
    cout << "3. Round Robin\n";
    cout << "4. SRTN\n";
    cout << "5. Priority Scheduling\n";
    cout << "Enter your choice: ";
    cin >> choice;

    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Enter arrival time for process " << i + 1 << ": ";
        cin >> processes[i].arrival_time;
        cout << "Enter burst time for process " << i + 1 << ": ";
        cin >> processes[i].burst_time;
        processes[i].remaining_time = processes[i].burst_time;
        if (choice == 5) {
            cout << "Enter priority for process " << i + 1 << ": ";
            cin >> processes[i].priority;
        }
    }

    if (choice == 3) {
            cout << "Enter the time quantum for Round Robin: ";
        cin >> quantum;
    }

    switch (choice) {
        case 1:
            calculateFCFS(processes);
            break;
        case 2:
            calculateSJF(processes);
            break;
        case 3:
            calculateRR(processes, quantum);
            break;
        case 4:
            calculateSRTN(processes);
            break;
        case 5:
            calculatePriority(processes);
            break;
        default:
            cout << "Invalid choice. Exiting...\n";
            return 1;
    }

    // Calculate and display average waiting time
    int total_waiting_time = 0;
    for (int i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
    }

    cout << "Average Waiting Time: " << static_cast<double>(total_waiting_time) / n << endl;

    return 0;
}
