--.open schlib.db
--.headers on
--.mode column

DROP TABLE IF EXISTS symbols;
DROP TABLE IF EXISTS pins;
DROP TABLE IF EXISTS footprints;
DROP TABLE IF EXISTS lib_shapes;
DROP TABLE IF EXISTS ref_shapes;
DROP TABLE IF EXISTS lib_shapes_inst;
DROP TABLE IF EXISTS ref_shapes_inst;

PRAGMA foreign_keys = on;

CREATE TABLE symbols (symbol_id TEXT PRIMARY KEY);
                    
CREATE TABLE pins (pin_id INTEGER PRIMARY KEY,
                   des TEXT,
                   name TEXT,
                   length NUMERIC DEFAULT 10,
                   des_style TEXT
                   des_xpos NUMERIC,
                   des_ypos NUMERIC,
                   des_rotation NUMERIC,
                   name_style TEXT,
                   name_xpos NUMERIC,
                   name_ypos NUMERIC,
                   name_rotation NUMERIC,
                   feature TEXT, -- somehow encode clock, dot, etc.
                   symbol_id TEXT,
                   FOREIGN KEY (symbol_id) REFERENCES symbols ON UPDATE CASCADE ON DELETE CASCADE);

-- Some shapes are defined locally in this library
CREATE TABLE lib_shapes (lib_shape_id INTEGER PRIMARY KEY,
                         name TEXT,
                         shape TEXT, -- svg or other
                         style TEXT); -- css
 
-- References to shapes in other libs
CREATE TABLE ref_shapes (ref_shape_id INTEGER PRIMARY KEY,
                         lib_path TEXT,
                         lib_name TEXT NOT NULL,
                         shape_name TEXT NOT NULL,
                         style TEXT); -- higher priority than built-in style

 -- This maps lib shapes to symbols
CREATE TABLE lib_shapes_inst (libshapes_inst_id INTEGER PRIMARY KEY,
                           lib_shape_id INTEGER,
                           symbol_id INTEGER,
                           xpos NUMERIC NOT NULL DEFAULT 0,
                           ypos NUMERIC NOT NULL DEFAULT 0,
                           xscale NUMERIC NOT NULL DEFAULT 1,
                           yscale NUMERIC NOT NULL DEFAULT 1,
                           FOREIGN KEY (lib_shape_id) REFERENCES lib_shapes ON UPDATE CASCADE ON DELETE CASCADE,
                           FOREIGN KEY (symbol_id)    REFERENCES symbols    ON UPDATE CASCADE ON DELETE CASCADE);
                        
 -- This maps ref shapes to symbols
CREATE TABLE ref_shapes_inst (refshapes_inst_id INTEGER PRIMARY KEY,
                           ref_shape_id INTEGER,
                           symbol_id INTEGER,
                           xpos NUMERIC NOT NULL DEFAULT 0,
                           ypos NUMERIC NOT NULL DEFAULT 0,
                           xscale NUMERIC NOT NULL DEFAULT 1,
                           yscale NUMERIC NOT NULL DEFAULT 1,
                           FOREIGN KEY (ref_shape_id) REFERENCES ref_shapes ON UPDATE CASCADE ON DELETE CASCADE
                           FOREIGN KEY (symbol_id)    REFERENCES symbols    ON UPDATE CASCADE ON DELETE CASCADE);
                        

INSERT INTO symbols (symbol_id) VALUES 
    ('cap_npol'),
    ('cap_pol'),
    ('res'),
    ('res_var'),
    ('res_var_3p'),
    ('inductor'),
    ('ferrite'),
    ('transformer'),
    ('npn'),
    ('pnp'),
    ('nfet'),
    ('pfet'),
    ('lt8pinabc');


INSERT INTO pins (des, name, symbol_id) VALUES
    (1,'', 'res'),
    (2,'', 'res'),
    (1,'', 'cap_pol'),
    (2,'', 'cap_pol'),
    (1,'', 'cap_npol'),
    (2,'', 'cap_npol'),
    (1,'F̅S̅y̅m̅b̅o̅l̅s̅','lt8pinabc'),
    (2,'RST','lt8pinabc'),
    (3,'SCL','lt8pinabc'),
    (4,'GND','lt8pinabc'),
    (5,'I̅N̅T̅','lt8pinabc'),
    (6,'CLK','lt8pinabc'),
    (7,'GPIO0','lt8pinabc'),
    (8,'GPIO1','lt8pinabc');

