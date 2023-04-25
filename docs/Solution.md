# Solution

We can achieve the solution for this problem in `O(n)`. We write a function that returns us the distance traveled by the rover and the index of the last gas station that it collected fuel from, given a particular starting index. The solution for the original problem is just to collect the distance from the output of the above function.

<details>
  <summary><b>Snippet for calculating the distance</b></summary>

  ```cpp
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
    long lastLocation, nextLocation, distanceToNextLocation;
    int lastStation, nextStation;

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

        // Travel to next gas station
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
```

</details>

<br>

# Solution to extension problem

To find the gas station from which the rover can travel the maximum distance, we can have 2 approaches. A simple and easy to understand approach solves this in `O(n^2)`. We calculate the distance that the rover can travel for `0 <= startingIndex < N`, and then just take the max value.


<details>
  <summary><b>Snippet for calculating the max distance with a naive approach</b></summary>

```cpp
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
```
</details>
<br>

A more efficient way finds the solution in `O(n)`. We start the journey at gas station on `startingIndex = 0`. We calculate the distance and the index of the last refueling stop. If the distance on the last journey is more than the max possible distance, we update the value of max possible distance and its starting index. If the index of the last refueling gas station is less than the `startingIndex`, or if it is `N - 1`, we have arrived at our solution and return the max possible distance along with its starting index. Conversely, if the index of the last refueling gas station is greater than or equal to the `startingIndex`, and if it is not `N - 1`, then we reset our journey and start from the next gas station. The following code explains this -


<details>
  <summary><b>Snippet for calculating the max distance efficiently</b></summary>


```cpp
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
```
</details>