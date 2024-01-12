global = {
  \key c \Major
  \time 4/4
  \version "2.22.1"
}

\parallelMusic voiceA,voiceB,voiceC,voiceD {
e'''4 d''' |
c'''4 b'' |
g''4 g'' |
c'4 g' |
}

\score {
          \new PianoStaff <<
             \new Staff {
               \global
               <<
                 \fixed c \voiceA
                 \\
                 \fixed c,  \voiceB
               >>
             }
             \new Staff {
               \global \clef bass
               <<
                 \fixed c, \voiceC
                 \\
                 \fixed c, \voiceD
               >>
             }
          >>
        }
        