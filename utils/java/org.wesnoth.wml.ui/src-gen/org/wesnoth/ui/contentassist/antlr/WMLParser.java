/*
* generated by Xtext
*/
package org.wesnoth.ui.contentassist.antlr;

import java.util.Collection;
import java.util.Map;
import java.util.HashMap;

import org.antlr.runtime.RecognitionException;
import org.eclipse.xtext.AbstractElement;
import org.eclipse.xtext.ui.editor.contentassist.antlr.AbstractContentAssistParser;
import org.eclipse.xtext.ui.editor.contentassist.antlr.FollowElement;
import org.eclipse.xtext.ui.editor.contentassist.antlr.internal.AbstractInternalContentAssistParser;

import com.google.inject.Inject;

import org.wesnoth.services.WMLGrammarAccess;

public class WMLParser extends AbstractContentAssistParser {
	
	@Inject
	private WMLGrammarAccess grammarAccess;
	
	private Map<AbstractElement, String> nameMappings;
	
	@Override
	protected org.wesnoth.ui.contentassist.antlr.internal.InternalWMLParser createParser() {
		org.wesnoth.ui.contentassist.antlr.internal.InternalWMLParser result = new org.wesnoth.ui.contentassist.antlr.internal.InternalWMLParser(null);
		result.setGrammarAccess(grammarAccess);
		return result;
	}
	
	@Override
	protected String getRuleName(AbstractElement element) {
		if (nameMappings == null) {
			nameMappings = new HashMap<AbstractElement, String>() {
				private static final long serialVersionUID = 1L;
				{
					put(grammarAccess.getWMLRootAccess().getAlternatives(), "rule__WMLRoot__Alternatives");
					put(grammarAccess.getWMLTagAccess().getAlternatives_4(), "rule__WMLTag__Alternatives_4");
					put(grammarAccess.getWMLAbstractMacroCallAccess().getAlternatives(), "rule__WMLAbstractMacroCall__Alternatives");
					put(grammarAccess.getWMLAbstractKeyValueAccess().getAlternatives(), "rule__WMLAbstractKeyValue__Alternatives");
					put(grammarAccess.getWMLANYVALUEAccess().getAlternatives(), "rule__WMLANYVALUE__Alternatives");
					put(grammarAccess.getWMLTagAccess().getGroup(), "rule__WMLTag__Group__0");
					put(grammarAccess.getWMLKeyAccess().getGroup(), "rule__WMLKey__Group__0");
					put(grammarAccess.getWMLKeyAccess().getGroup_3(), "rule__WMLKey__Group_3__0");
					put(grammarAccess.getWMLMacroIncludeAccess().getGroup(), "rule__WMLMacroInclude__Group__0");
					put(grammarAccess.getWMLMacroCallAccess().getGroup(), "rule__WMLMacroCall__Group__0");
					put(grammarAccess.getTSTRINGAccess().getGroup(), "rule__TSTRING__Group__0");
					put(grammarAccess.getWMLRootAccess().getTagsAssignment_0(), "rule__WMLRoot__TagsAssignment_0");
					put(grammarAccess.getWMLRootAccess().getMacroCallsAssignment_1(), "rule__WMLRoot__MacroCallsAssignment_1");
					put(grammarAccess.getWMLTagAccess().getPlusAssignment_1(), "rule__WMLTag__PlusAssignment_1");
					put(grammarAccess.getWMLTagAccess().getNameAssignment_2(), "rule__WMLTag__NameAssignment_2");
					put(grammarAccess.getWMLTagAccess().getTagsAssignment_4_0(), "rule__WMLTag__TagsAssignment_4_0");
					put(grammarAccess.getWMLTagAccess().getKeysAssignment_4_1(), "rule__WMLTag__KeysAssignment_4_1");
					put(grammarAccess.getWMLTagAccess().getMacroCallsAssignment_4_2(), "rule__WMLTag__MacroCallsAssignment_4_2");
					put(grammarAccess.getWMLTagAccess().getEndNameAssignment_6(), "rule__WMLTag__EndNameAssignment_6");
					put(grammarAccess.getWMLKeyAccess().getNameAssignment_0(), "rule__WMLKey__NameAssignment_0");
					put(grammarAccess.getWMLKeyAccess().getValueAssignment_2(), "rule__WMLKey__ValueAssignment_2");
					put(grammarAccess.getWMLKeyAccess().getExtraArgsAssignment_3_1(), "rule__WMLKey__ExtraArgsAssignment_3_1");
					put(grammarAccess.getWMLMacroIncludeAccess().getPathAssignment_2(), "rule__WMLMacroInclude__PathAssignment_2");
					put(grammarAccess.getWMLMacroCallAccess().getNameAssignment_1(), "rule__WMLMacroCall__NameAssignment_1");
					put(grammarAccess.getWMLMacroCallAccess().getParamsAssignment_2(), "rule__WMLMacroCall__ParamsAssignment_2");
				}
			};
		}
		return nameMappings.get(element);
	}
	
	@Override
	protected Collection<FollowElement> getFollowElements(AbstractInternalContentAssistParser parser) {
		try {
			org.wesnoth.ui.contentassist.antlr.internal.InternalWMLParser typedParser = (org.wesnoth.ui.contentassist.antlr.internal.InternalWMLParser) parser;
			typedParser.entryRuleWMLRoot();
			return typedParser.getFollowElements();
		} catch(RecognitionException ex) {
			throw new RuntimeException(ex);
		}		
	}
	
	@Override
	protected String[] getInitialHiddenTokens() {
		return new String[] { "RULE_WS", "RULE_SL_COMMENT" };
	}
	
	public WMLGrammarAccess getGrammarAccess() {
		return this.grammarAccess;
	}
	
	public void setGrammarAccess(WMLGrammarAccess grammarAccess) {
		this.grammarAccess = grammarAccess;
	}
}
