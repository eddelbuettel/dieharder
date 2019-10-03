(TeX-add-style-hook "RDieHarder"
 (lambda ()
    (LaTeX-add-bibliographies)
    (TeX-add-symbols
     '("pkg" 1)
     "diehard"
     "code"
     "proglang")
    (TeX-run-style-hooks
     "color"
     "natbib"
     "authoryear"
     "round"
     "rotating"
     "graphicx"
     "booktabs"
     "hyperref"
     "url"
     "vmargin"
     "latex2e"
     "art12"
     "article"
     "12pt")))

