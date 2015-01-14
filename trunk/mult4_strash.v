// Benchmark "mult4" written by ABC on Tue Jan 13 21:29:40 2015

module mult4 ( 
    a0, a1, a2, a3, b0, b1, b2, b3,
    z0, z1, z2, z3, z4, z5, z6, z7  );
  input  a0, a1, a2, a3, b0, b1, b2, b3;
  output z0, z1, z2, z3, z4, z5, z6, z7;
  wire n18, n19, n20, n21, n23, n24, n25, n26, n27, n28, n29, n30, n31, n32,
    n33, n34, n36, n37, n38, n39, n40, n41, n42, n43, n44, n45, n46, n47,
    n48, n49, n50, n51, n52, n53, n54, n55, n56, n57, n58, n60, n61, n62,
    n63, n64, n65, n66, n67, n68, n69, n70, n71, n72, n73, n74, n75, n76,
    n77, n78, n79, n80, n81, n83, n84, n85, n86, n87, n88, n89, n90, n91,
    n92, n93, n94, n95, n96, n97, n98, n99, n100, n101, n102, n103, n104,
    n105, n106, n108, n109, n110, n111, n112, n113, n114, n115, n116, n117,
    n118, n119, n120, n121, n122, n123, n125, n126, n127, n128;
  assign z0 = a0 & b0;
  assign n18 = a1 & b0;
  assign n19 = a0 & b1;
  assign n20 = ~n18 & n19;
  assign n21 = n18 & ~n19;
  assign z1 = n20 | n21;
  assign n23 = n18 & n19;
  assign n24 = a1 & b2;
  assign n25 = a0 & b2;
  assign n26 = ~n24 & n25;
  assign n27 = n24 & ~n25;
  assign n28 = ~n26 & ~n27;
  assign n29 = n23 & n28;
  assign n30 = ~n23 & ~n28;
  assign n31 = ~n29 & ~n30;
  assign n32 = a2 & b0;
  assign n33 = n31 & n32;
  assign n34 = ~n31 & ~n32;
  assign z2 = n33 | n34;
  assign n36 = n24 & n25;
  assign n37 = n23 & n24;
  assign n38 = ~n36 & ~n37;
  assign n39 = n23 & n25;
  assign n40 = n38 & ~n39;
  assign n41 = a0 & b3;
  assign n42 = ~n24 & n41;
  assign n43 = n24 & ~n41;
  assign n44 = ~n42 & ~n43;
  assign n45 = ~n40 & n44;
  assign n46 = n40 & ~n44;
  assign n47 = ~n45 & ~n46;
  assign n48 = ~n31 & n32;
  assign n49 = a2 & b1;
  assign n50 = n47 & n49;
  assign n51 = ~n47 & ~n49;
  assign n52 = ~n50 & ~n51;
  assign n53 = n48 & n52;
  assign n54 = ~n48 & ~n52;
  assign n55 = ~n53 & ~n54;
  assign n56 = a3 & b0;
  assign n57 = n55 & n56;
  assign n58 = ~n55 & ~n56;
  assign z3 = n57 | n58;
  assign n60 = n24 & n41;
  assign n61 = n24 & ~n40;
  assign n62 = ~n60 & ~n61;
  assign n63 = ~n40 & n41;
  assign n64 = n62 & ~n63;
  assign n65 = a1 & b3;
  assign n66 = ~n64 & ~n65;
  assign n67 = n64 & n65;
  assign n68 = ~n66 & ~n67;
  assign n69 = n49 & n68;
  assign n70 = ~n49 & ~n68;
  assign n71 = ~n69 & ~n70;
  assign n72 = n48 & n71;
  assign n73 = ~n48 & ~n71;
  assign n74 = ~n72 & ~n73;
  assign n75 = ~n55 & n56;
  assign n76 = a3 & b1;
  assign n77 = n74 & n76;
  assign n78 = ~n74 & ~n76;
  assign n79 = ~n77 & ~n78;
  assign n80 = n75 & n79;
  assign n81 = ~n75 & ~n79;
  assign z4 = n80 | n81;
  assign n83 = ~n64 & n65;
  assign n84 = n48 & n49;
  assign n85 = n49 & ~n68;
  assign n86 = n48 & ~n68;
  assign n87 = ~n85 & ~n86;
  assign n88 = ~n84 & n87;
  assign n89 = a2 & b3;
  assign n90 = ~n83 & n89;
  assign n91 = n83 & ~n89;
  assign n92 = ~n90 & ~n91;
  assign n93 = ~n88 & n92;
  assign n94 = n88 & ~n92;
  assign n95 = ~n93 & ~n94;
  assign n96 = n75 & n76;
  assign n97 = ~n74 & n76;
  assign n98 = ~n74 & n75;
  assign n99 = ~n97 & ~n98;
  assign n100 = ~n96 & n99;
  assign n101 = a3 & b2;
  assign n102 = n95 & n101;
  assign n103 = ~n95 & ~n101;
  assign n104 = ~n102 & ~n103;
  assign n105 = ~n100 & n104;
  assign n106 = n100 & ~n104;
  assign z5 = n105 | n106;
  assign n108 = ~n88 & n89;
  assign n109 = n83 & n89;
  assign n110 = n83 & ~n88;
  assign n111 = ~n109 & ~n110;
  assign n112 = ~n108 & n111;
  assign n113 = ~n100 & n101;
  assign n114 = ~n95 & n101;
  assign n115 = ~n95 & ~n100;
  assign n116 = ~n114 & ~n115;
  assign n117 = ~n113 & n116;
  assign n118 = a3 & b3;
  assign n119 = n112 & n118;
  assign n120 = ~n112 & ~n118;
  assign n121 = ~n119 & ~n120;
  assign n122 = ~n117 & n121;
  assign n123 = n117 & ~n121;
  assign z6 = n122 | n123;
  assign n125 = ~n117 & n118;
  assign n126 = ~n112 & n118;
  assign n127 = ~n112 & ~n117;
  assign n128 = ~n126 & ~n127;
  assign z7 = n125 | ~n128;
endmodule


