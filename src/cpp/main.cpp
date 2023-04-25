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
// First element of the tuple is the location of the gas station
// Second element of the tuple is the number of miles that the rover can travel with the amount of gas available at that gas station
// Returns the circumference, number of gas stations, vector of gas station details, and the index of gas station from which to start the journey
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
// Returns the total distance traveled by the rover, along with the index of the last gas station that it refueled at.

// Example Input:
// circumference = 100
// N = 5
// gasStations = [(0, 30), (20, 10), (40, 20), (60, 50), (80, 10)]
// startingIndex = 3
// Expected Output: (120, 2)
tuple<long, int> getDistanceWithLastGasStationIndex(long circumference, int N, const vector<tuple<long, long>> &gasStations, int startingIndex)
{
    long capacity = 0, distanceTraveled = 0;
    int lastStation, nextStation;
    long lastLocation, nextLocation, distanceToNextLocation;

    for (int i = 0; i < N; i++)
    {
        lastStation = (i + startingIndex) % N;
        nextStation = (lastStation + 1) % N;

        // Collect all the available gas
        capacity += get<1>(gasStations[lastStation]);

        // Calculate distance to next location
        lastLocation = get<0>(gasStations[lastStation]);
        nextLocation = get<0>(gasStations[nextStation]);
        distanceToNextLocation = (nextLocation - lastLocation + circumference) % circumference;

        // Check if rover can reach the next gas station
        if (capacity < distanceToNextLocation)
        {
            // Drive as much as possible before the gas tank is empty
            distanceTraveled += capacity;

            // lastStation represents the last gas station where the rover fueled up
            return tuple(distanceTraveled, lastStation);
        }

        // Travel to the next gas station
        distanceTraveled += distanceToNextLocation;
        capacity -= distanceToNextLocation;
    }

    // Run till gas tank is empty.
    return tuple(distanceTraveled + capacity, lastStation);
}

// Returns the total distance traveled by the rover from the gasStation at startingIndex
// Example Input:
// circumference = 100
// N = 5
// gasStations = [(0, 30), (20, 10), (40, 20), (60, 50), (80, 10)]
// startingIndex = 3
// Expected Output: 120
long getDistance(long circumference, int N, const vector<tuple<long, long>> &gasStations, int startingIndex)
{
    return get<0>(getDistanceWithLastGasStationIndex(circumference, N, gasStations, startingIndex));
}

// Calculates the max possible distance that the rover can cover with the given configuration
// Returns the distance and the index of the gas station to start from to achieve this distance
// Example Input:
// circumference = 6
// N = 3
// gasStations = [(0, 1), (2, 1), (4, 10)]
// Expected Output: (12, 2)
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

// Calculates the max possible distance that the rover can cover with the given configuration
// Example Input:
// circumference = 6
// N = 3
// gasStations = [(0, 1), (2, 1), (4, 10)]
// Expected Output: 12
long getMaxPossibleDistance(long circumference, int N, const vector<tuple<long, long>> &gasStations)
{
    return get<0>(getMaxPossibleDistanceWithStartingIndex(circumference, N, gasStations));
}

// Calculates distances that rover can travel if it were to start at each gas station
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