global = {
  \key c \Major
  \time 4/4
  \version "2.22.1"
}

\parallelMusic voiceA,voiceB,voiceC,voiceD {
b''4 ees''' |
ges''4 ges'' |
ees''4 b' |
b4 b |
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
        