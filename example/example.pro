<?xml version="1.0" encoding="UTF-8"?>
<project>
 <assembly>
  <group depth_sorted_bin_state="1" name="Root" depth_sorted_bin_value="2">
   <group depth_sorted_bin_state="0" name="Group" depth_sorted_bin_value="1">
    <file file="pyramid.osgt" depth_sorted_bin_state="0" name="File" depth_sorted_bin_value="1"/>
   </group>
   <pat az="0.000000000000" pz="0.000000" convention="0" depth_sorted_bin_state="0" ax="0.000000000000" name="PAT" px="-2.000000" depth_sorted_bin_value="1" py="0.000000" ay="0.000000000000">
    <box depth_sorted_bin_state="0" name="Box" depth_sorted_bin_value="1"/>
   </pat>
   <lod interval_2="60" interval_1="40" depth_sorted_bin_state="0" name="LOD" depth_sorted_bin_value="1" interval_0="20">
    <pat az="0.000000000000" pz="2.000000" convention="0" depth_sorted_bin_state="0" ax="0.000000000000" name="PAT" px="0.000000" depth_sorted_bin_value="1" py="0.000000" ay="0.000000000000">
     <box depth_sorted_bin_state="0" name="Box" depth_sorted_bin_value="1"/>
    </pat>
    <pat az="0.000000000000" pz="4.000000" convention="0" depth_sorted_bin_state="0" ax="0.000000000000" name="PAT" px="0.000000" depth_sorted_bin_value="1" py="0.000000" ay="0.000000000000">
     <box depth_sorted_bin_state="0" name="Box" depth_sorted_bin_value="1"/>
    </pat>
    <pat az="0.000000000000" pz="6.000000" convention="0" depth_sorted_bin_state="0" ax="0.000000000000" name="PAT" px="0.000000" depth_sorted_bin_value="1" py="0.000000" ay="0.000000000000">
     <box depth_sorted_bin_state="0" name="Box" depth_sorted_bin_value="1"/>
    </pat>
   </lod>
   <switch visible="1" depth_sorted_bin_state="0" name="Switch" depth_sorted_bin_value="1">
    <pat az="0.000000000000" pz="0.000000" convention="0" depth_sorted_bin_state="0" ax="0.000000000000" name="PAT" px="0.000000" depth_sorted_bin_value="1" py="-2.000000" ay="0.000000000000">
     <box depth_sorted_bin_state="0" name="Box" depth_sorted_bin_value="1"/>
    </pat>
   </switch>
   <rotor az="0.000000000000" pz="-2.000000" direction="0" convention="0" file_blade="blade.osgt" file_shaft="shaft.osgt" depth_sorted_bin_state="0" ax="0.000000000000" name="Rotor" blades_no="4" px="0.000000" hinge_offset="1" depth_sorted_bin_value="1" py="0.000000" ay="0.000000000000"/>
   <matrix_trans az="0.000000000000" pz="-1.000000" depth_sorted_bin_state="0" ax="0.000000000000" name="Matrix Transform" px="0.000000" depth_sorted_bin_value="1" py="0.000000" ay="0.000000000000">
    <animation t_max="1.0000" t_min="0.0000">
     <keyframe az="0.000000000000" pz="-1.000000" t="0.0000" ax="0.000000000000" px="0.000000" py="0.000000" ay="0.000000000000"/>
     <keyframe az="0.000000000000" pz="-1.000000" t="0.5000" ax="0.000000000000" px="1.000000" py="0.000000" ay="0.000000000000"/>
     <keyframe az="0.000000000000" pz="-1.000000" t="1.0000" ax="0.000000000000" px="2.000000" py="0.000000" ay="0.000000000000"/>
    </animation>
    <box depth_sorted_bin_state="0" name="Box" depth_sorted_bin_value="1"/>
   </matrix_trans>
   <flols az="0.000000000000" pz="0.000000" convention="0" depth_sorted_bin_state="0" ax="0.000000000000" name="FLOLS" px="0.000000" depth_sorted_bin_value="1" py="0.000000" ay="0.000000000000"/>
  </group>
 </assembly>
 <playback time_start="0.000000" time_end="1.000000" speed="0.500000" current_time="0.500000"/>
</project>
