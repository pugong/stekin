WORKDIR=parser
AWK=gawk

include misc/mf-template.mk

parser:yy-misc.d syntax-types.d lexical syntactically

syntactically:
	$(RESOLVE_DEP) $(WORKDIR)/syn-include.h | $(AWK) -f $(WORKDIR)/syn-script > $(MKTMP)
	make -f $(MKTMP)

lexical:syntactically
	$(RESOLVE_DEP) $(WORKDIR)/lex-include.h | $(AWK) -f $(WORKDIR)/lex-script > $(MKTMP)
	make -f $(MKTMP)

clean:
	rm -f $(WORKDIR)/syn.cpp
	rm -f $(WORKDIR)/syn.hpp
	rm -f $(WORKDIR)/lex.cpp
	rm -f $(WORKDIR)/*.o
