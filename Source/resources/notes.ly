global = {
  \key d \major
  \time 4/4
  \version "2.22.1"
}

\parallelMusic voiceA,voiceB,voiceC,voiceD {
fis'''4 e''' e''' |
b''4 b'' cis''' |
dis''4 g'' g'' |
b'4 e' cis' |
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
        