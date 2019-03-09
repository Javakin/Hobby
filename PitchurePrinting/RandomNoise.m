clear all
close all
clc

%%
feeder = [1,0.5, 0.8,0.5, 0.6,0.5, 0.5,0.5, 0.5,0.6, 0.5,0.8, 0.5,1
                              ]
                          %%


for n = 1:length(feeder)
    feeder(n) = feeder(n) + (rand()-0.5)*2*0.05;
    disp(str(feeder(n)))
    disp(', ');
end
