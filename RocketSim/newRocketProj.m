clear; clc;

g = 9.81; % Gravity 
mDry = 1.05; % Mass
mProp = 0.5;
mTotal = mDry + mProp;
a = .0045;
data = readtable('betterData.csv');
thrustMatrix = [data{:, 1}, data{:, 2}];
lGimbal = 1.0;

