# high level synthesis module for low-latency neural signal processors

This repo contains all the Vivado-HLS modules used in Project Xike. 

The first  module is **spkPack**. It takes muap stream data and extract spk waveform from it. 

The second module is **spkTF**.   It transform the spk waveform into low-D space. (The same transformation for clustering) 

The third  module is **spkClf**.  It use kNN model to do classification on low-D space. 

