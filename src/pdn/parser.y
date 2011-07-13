%%

PDN_database            
:       /* empty */        
|       PDN_database PDN_game
; 
                
PDN_game                
:       tag_section movetext_section
;
                
tag_section
:       /* empty */
|       tag_section tag_pair
;
                
tag_pair
:       LBRACK tag_name tag_value RBRACK
;

tag_name 
:       identifier
;

tag_value
:       string
;

movetext_section
:       element_sequence game_termination
;

element_sequence
:       /* empty */
|       element_sequence element
|       element_sequence recursive_variation
;

element
:       move_number_indication SAN_move numeric_annotation_glyph
;

recursive_varation
:       LPAREN element_sequence RPAREN
;

game_termination
:       WHITE_WIN
|       BLACK_WIN
| DRAW
| NORESULT
;

%%