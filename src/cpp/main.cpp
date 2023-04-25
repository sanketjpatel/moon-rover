/*  main.cpp
 *  Author: Sanket Patel
 *  Date: 04/24/2023
 */
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

// Read the route details from the input file
// A gas station's details are in a tuple
// First element of tuple is the location of the gas station
// Second element of tuple is the miles the vehicle can travel with the amount of gas available at that gas station
// Returns the circumference, number of gas stations, vector of gas station details, and index of gas station to start the journey
tuple<long, int, vector<tuple<long, long>>, int> getRouteDetails(fstream &inputFile)
{
    vector<tuple<long, long>> gasStations;
    string temp;

    long location, capacity;

    getline(inputFile, temp);
    long circumference = stol(temp);

    getline(inputFile, temp);
    int numStations = stoi(temp);

    for (int i = 0; i < numStations; i++)
    {
        getline(inputFile, temp, ' ');
        location = stol(temp);

        getline(inputFile, temp, '\n');
        capacity = stol(temp);

        gasStations.push_back(tuple(location, capacity));
    }
    
    getline(inputFile, temp);
    int startingIndex = stoi(temp);

    return tuple(circumference, numStations, gasStations, startingIndex);
}

// gasStations is expected to be sorted by location (the value of first element of the tuples in it)
// Returns the total distance traveled by the truck, along with the index of the last gas station that it refueled.
tuple<long, int> getDistanceWithLastGasStationIndex(long circumference, int N, const vector<tuple<long, long>> &gasStations, int startingIndex)
{
    long capacity = 0;
    long distance = 0;
    long currentLocation, nextLocation, distanceToNextLocation;
    int index, nextIndex;

    for (int i = 0; i < N; i++)
    {
        index = (i + startingIndex) % N;
        nextIndex = (index + 1) % N;

        // Collect all the available gas
        capacity += get<1>(gasStations[index]);

        // Calculate distance to next location
        currentLocation = get<0>(gasStations[index]);
        nextLocation = get<0>(gasStations[nextIndex]);
        distanceToNextLocation = (nextLocation - currentLocation + circumference) % circumference;

        // Check if truck can reach next gas station
        if (capacity < distanceToNextLocation)
        {
            // Drive as much as possible before gas tank is empty
            distance += capacity;

            // index represents the last gas station where the vehicle fueled up
            return tuple(distance, index);
        }

        // Travel to next gas station
        distance += distanceToNextLocation;
        capacity -= distanceToNextLocation;
    }
    // The vehicle is at the starting gas station now.
    int lastGasStation = (startingIndex - 1 + N) % N;

    // Run till gas tank is empty.
    return tuple(distance + capacity, lastGasStation);
}

long getDistance(long circumference, int N, const vector<tuple<long, long>> &gasStations, int startingIndex)
{
    return get<0>(getDistanceWithLastGasStationIndex(circumference, N, gasStations, startingIndex));
}

tuple<long, int> getMaxPossibleDistanceWithStartingIndex(long circumference, int N, const vector<tuple<long, long>> &gasStations)
{
    long maxDistance = 0, distance = 0;
    int maxDistanceStartingIndex = 0, startingIndex = 0;
    int lastGasStationIndex = -1;

    do {
        startingIndex = lastGasStationIndex + 1 % N;
        tie(distance, lastGasStationIndex) = getDistanceWithLastGasStationIndex(circumference, N, gasStations, startingIndex);
        if (distance > maxDistance)
        {
            maxDistance = distance;
            maxDistanceStartingIndex = startingIndex;
        }
    } while ((lastGasStationIndex >= startingIndex) && (lastGasStationIndex != N - 1));

    return tuple(maxDistance, maxDistanceStartingIndex);
}

// Calculates distances that truck can travel if it were to start at each gas station
// returns the max distance and its corresponding starting index
tuple<long, int> getMaxPossibleDistanceNaive(long circumference, int N, const vector<tuple<long, long>> &gasStations)
{
    long maxDistance = 0, currentDistance = 0;
    int maxStartingIndex = 0;
    for (int i = 0; i < N; i++)
    {
        currentDistance = getDistance(circumference, N, gasStations, i);
        if (currentDistance > maxDistance)
        {
            maxDistance = currentDistance;
            maxStartingIndex = i;
        }
    }

    return tuple(maxDistance, maxStartingIndex);
}

void writeDistances(const string &filepath, const vector<long> &distances)
{
    fstream outputFile;
    outputFile.open(filepath, ios::out);
    if (outputFile.is_open())
    {
        for (auto const &distance : distances)
        {
            outputFile << distance << endl;
        }
        outputFile.close();
    }
}

void writeDistancesWithStartingIndex(const string &filepath, const vector<tuple<long, int>> &results)
{
    fstream outputFile;
    outputFile.open(filepath, ios::out);
    if (outputFile.is_open())
    {
        for (auto const &tupleEntry : results)
        {
            outputFile << get<0>(tupleEntry) << " " << get<1>(tupleEntry) << endl;
        }
        outputFile.close();
    }
}

int main()
{
    int T;
    const string inputFilePath = "resources/input.txt";
    vector<long> results;
    vector<tuple<long, int>> resultsMaxDistance;
    vector<tuple<long, int>> resultsMaxDistanceNaive;

    const string outputFilePath = "outputs/results.txt";
    const string outputMaxDistanceFilePath = "outputs/results-max.txt";
    const string outputMaxDistanceNaiveFilePath = "outputs/results-max-naive.txt";

    fstream inputFile;
    inputFile.open(inputFilePath, ios::in);
    if (inputFile.is_open())
    {
        string t;
        getline(inputFile, t);
        T = stoi(t);
        // Loop through each test case and calculate its results
        for (int i = 0; i < T; i++)
        {
            long circumference;
            int numStations;
            int startingIndex;
            vector<tuple<long, long>> gasStations;
            tie(circumference, numStations, gasStations, startingIndex) = getRouteDetails(inputFile);
            results.push_back(getDistance(circumference, numStations, gasStations, startingIndex));
            resultsMaxDistance.push_back(getMaxPossibleDistanceWithStartingIndex(circumference, numStations, gasStations));
            resultsMaxDistanceNaive.push_back(getMaxPossibleDistanceNaive(circumference, numStations, gasStations));
        }
        inputFile.close();
    }

    writeDistances(outputFilePath, results);
    writeDistancesWithStartingIndex(outputMaxDistanceFilePath, resultsMaxDistance);
    writeDistancesWithStartingIndex(outputMaxDistanceNaiveFilePath, resultsMaxDistanceNaive);

    return 0;
}