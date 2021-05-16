/*
 *
 * This file is part of LautIO
 *
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */
 
 $fn = 60;
 
 plate_width           = 155.50;
 plate_height          = 52.00;
 
 screw_size            = 3.10;
 screw_distance        = 4.00;
 middle_screw_distance = 78.00;
 
 cutout_left_width     = 28.00;
 cutout_right_width    = 29.50;
 
 module pipemount() {
     difference() {
        
         // ground plane
         square([plate_width, plate_height]);
         
         union() {
             
             // screw holes
             translate([screw_distance, screw_distance])
             circle(d=screw_size);
             
             translate([screw_distance + middle_screw_distance, screw_distance])
             circle(d=screw_size);
             
             translate([plate_width - screw_distance, screw_distance])
             circle(d=screw_size);
             
             translate([plate_width - screw_distance, plate_height - screw_distance])
             circle(d=screw_size);
             
             translate([screw_distance, plate_height - screw_distance])
             circle(d=screw_size);
             
             translate([screw_distance + middle_screw_distance, plate_height - screw_distance])
             circle(d=screw_size);
             
             // left cutout
             translate([0, screw_distance * 2])
             square([cutout_left_width, plate_height - (screw_distance * 4)]);
             
             // right cutout
             translate([plate_width - cutout_right_width, screw_distance * 2])
             square([cutout_right_width, plate_height - (screw_distance * 4)]);
             
             
         }
     }
     
 }
 
 pipemount();