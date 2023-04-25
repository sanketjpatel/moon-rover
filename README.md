# Moon Rover

You are traveling along a circular road. The circumference of the circle is `L`. Along the route, you have `N` gas stations. At each gas station, you have some amount of gas available. If you are given the index number of a gas station from which you can start the journey, calculate the distance that you can travel. You are starting with an empty gas tank. The vehicle has infinite capacity. The gas station does not replenish the gas once you have filled up from it.

## Input

First line represents `T`, the number of test cases.

Each test case consists for `N+3` lines.

First line of a test case represents `L`, the circumference of the circle.

Second line represents `N`, the number of gas stations.

Next `N` lines represent the details of the gas stations. 

The details of a gas station can be represented by two space separated numbers, where the first number indicates the location of the gas station in miles. The second number indicates miles that you can travel with the gas available at that location.

Last line represents `i`, the index number of the gas station from which you will start the journey.

## Expected Output

`T` lines.

Each line represents the distance that can be traveled.

## Example:

Say your input is
```
1
100
5
3
0 30
20 10
40 20
60 50
80 10
```
First line says you are going to run `1` test case.

Second line says the circumference of the circle is `100` miles.

Third line says you have `5` gas stations.

Fourth line says you will start from gas station at index `3`.

The details of gas stations are as follows:
```
Gas station at index 0: Located at mile marker 0,  Available gas equivalent to 30 miles
Gas station at index 1: Located at mile marker 20, Available gas equivalent to 10 miles
Gas station at index 2: Located at mile marker 40, Available gas equivalent to 20 miles
Gas station at index 3: Located at mile marker 60, Available gas equivalent to 50 miles
Gas station at index 4: Located at mile marker 80, Available gas equivalent to 10 miles
```

Your output should be the following
```
120
```

Explanation:

You will start at the gas station on mile marker `60`. 

Fill up all the available gas. Now the vehicle has a capacity to travel `50` miles.

You travel `20` miles, and reach the next gas station. 

The capacity of vehicle has reduced to `30` miles now.

Fill up the available gas (`10`). The capacity is now `40` miles. Your current location is at mile marker `80`. 

The vehicle continues its journey for `20` miles and reaches the gas station at mile marker `0`. (The vehicle route is a circle of length `100` miles).

Capacity is now `20`. The vehicle does another refill of `30`, and continues forward with the journey...

As the vehicle keeps going forward, it will eventually reach the gas station at mile marker `40` and fill up all the available gas.

The vehicle would have traveled `80` miles so far, and it would have a capacity to travel `40` more miles.

Since none of the gas stations have any more gas available, the vehicle will run for `40` more miles and run out of gas.

Total distance traveled by the vehicle is `120` miles.


# Contributing

You may refer to [launch configurations](docs/Launch.md) used for this project. Please submit issues/pull requests and we can collaborate :slightly_smiling_face:

Sample inputs can be found in `resources/`

Sample outputs are under the `outputs/`

Cheers!
