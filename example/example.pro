<?xml version="1.0" encoding="UTF-8"?>
<project>
 <assembly>
  <group depth_sorted_bin_value="2" name="Root" depth_sorted_bin_state="1">
   <group depth_sorted_bin_value="1" name="Group" depth_sorted_bin_state="0">
    <file depth_sorted_bin_value="1" file="pyramid.osgt" name="File" depth_sorted_bin_state="0"/>
   </group>
   <pat anim_enabled="0" pz="0.000000" depth_sorted_bin_value="1" px="-2.000000" ay="0.000000000000" name="PAT" py="0.000000" depth_sorted_bin_state="0" ax="0.000000000000" convention="0" az="0.000000000000">
    <box depth_sorted_bin_value="1" name="Box" depth_sorted_bin_state="0"/>
   </pat>
   <lod interval_0="20" depth_sorted_bin_value="1" interval_1="40" name="LOD" depth_sorted_bin_state="0" interval_2="60">
    <pat anim_enabled="0" pz="2.000000" depth_sorted_bin_value="1" px="0.000000" ay="0.000000000000" name="PAT" py="0.000000" depth_sorted_bin_state="0" ax="0.000000000000" convention="0" az="0.000000000000">
     <box depth_sorted_bin_value="1" name="Box" depth_sorted_bin_state="0"/>
    </pat>
    <pat anim_enabled="0" pz="4.000000" depth_sorted_bin_value="1" px="0.000000" ay="0.000000000000" name="PAT" py="0.000000" depth_sorted_bin_state="0" ax="0.000000000000" convention="0" az="0.000000000000">
     <box depth_sorted_bin_value="1" name="Box" depth_sorted_bin_state="0"/>
    </pat>
    <pat anim_enabled="0" pz="6.000000" depth_sorted_bin_value="1" px="0.000000" ay="0.000000000000" name="PAT" py="0.000000" depth_sorted_bin_state="0" ax="0.000000000000" convention="0" az="0.000000000000">
     <box depth_sorted_bin_value="1" name="Box" depth_sorted_bin_state="0"/>
    </pat>
   </lod>
   <switch depth_sorted_bin_value="1" visible="1" name="Switch" depth_sorted_bin_state="0">
    <pat anim_enabled="0" pz="0.000000" depth_sorted_bin_value="1" px="0.000000" ay="0.000000000000" name="PAT" py="-2.000000" depth_sorted_bin_state="0" ax="0.000000000000" convention="0" az="0.000000000000">
     <box depth_sorted_bin_value="1" name="Box" depth_sorted_bin_state="0"/>
    </pat>
   </switch>
   <rotor direction="0" blades_no="4" anim_enabled="0" pz="-2.000000" depth_sorted_bin_value="1" hinge_offset="1" file_shaft="shaft.osgt" px="0.000000" ay="0.000000000000" file_blade="blade.osgt" name="Rotor" py="0.000000" depth_sorted_bin_state="0" ax="0.000000000000" convention="0" az="0.000000000000"/>
   <matrix_trans anim_enabled="1" pz="-1.000000" depth_sorted_bin_value="1" px="0.000000" ay="0.000000000000" name="Matrix Transform" py="0.000000" depth_sorted_bin_state="0" ax="0.000000000000" az="0.000000000000">
    <animation t_min="0.0000" t_max="1.0000">
     <keyframe t="0.0000" pz="-1.000000" px="0.000000" ay="0.000000000000" py="0.000000" ax="0.000000000000" az="0.000000000000"/>
     <keyframe t="0.5000" pz="-1.000000" px="1.000000" ay="0.000000000000" py="0.000000" ax="0.000000000000" az="0.000000000000"/>
     <keyframe t="1.0000" pz="-1.000000" px="2.000000" ay="0.000000000000" py="0.000000" ax="0.000000000000" az="0.000000000000"/>
    </animation>
    <box depth_sorted_bin_value="1" name="Box" depth_sorted_bin_state="0"/>
   </matrix_trans>
   <flols anim_enabled="0" pz="0.000000" depth_sorted_bin_value="1" px="0.000000" ay="0.000000000000" name="FLOLS" py="0.000000" depth_sorted_bin_state="0" ax="0.000000000000" convention="0" az="0.000000000000"/>
  </group>
 </assembly>
 <playback current_time="0.500000" time_end="1.000000" time_start="0.000000" speed="0.500000"/>
</project>
