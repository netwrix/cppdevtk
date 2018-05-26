/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2018 CoSoSys Ltd <info@cososys.com>\n
/// Licensed under the Apache License, Version 2.0 (the "License");\n
/// you may not use this file except in compliance with the License.\n
/// You may obtain a copy of the License at\n
/// http://www.apache.org/licenses/LICENSE-2.0\n
/// Unless required by applicable law or agreed to in writing, software\n
/// distributed under the License is distributed on an "AS IS" BASIS,\n
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
/// See the License for the specific language governing permissions and\n
/// limitations under the License.\n
/// Please see the file COPYING.
///
/// \authors Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <cppdevtk/base/config.hpp>
#if (CPPDEVTK_DISABLE_CPPDEVTK_WARNINGS)
#	pragma warning(disable: 4061)	// C4061: enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
#endif

#include <cppdevtk/base/locale_to_lang_id_win.hpp>
#if (!CPPDEVTK_PLATFORM_WINDOWS)
#	error "This file is Windows specific!!!"
#endif
#include <cppdevtk/base/cassert.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/string_utils.hpp>

#include <QtCore/QLocale>
#include <QtCore/QtGlobal>

#include <windows.h>


namespace cppdevtk {
namespace base {


::cppdevtk::base::word LocaleToLangId(const QLocale& locale) {
	WORD langId = 0;
	
	switch (locale.language()) {
		case QLocale::AnyLanguage:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
					break;
				default:
					langId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::C:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
					break;
				default:
					langId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Afrikaans:
			CPPDEVTK_ASSERT(locale.country() == QLocale::SouthAfrica);
			langId = MAKELANGID(LANG_AFRIKAANS, SUBLANG_AFRIKAANS_SOUTH_AFRICA);
			break;
		case QLocale::Albanian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Albania);
			langId = MAKELANGID(LANG_ALBANIAN, SUBLANG_ALBANIAN_ALBANIA);
			break;
		case QLocale::Amharic:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Ethiopia);
			langId = MAKELANGID(LANG_AMHARIC, SUBLANG_AMHARIC_ETHIOPIA);
			break;
		case QLocale::Arabic:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_NEUTRAL);
					break;
				case QLocale::Algeria:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_ALGERIA);
					break;
				case QLocale::Bahrain:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_BAHRAIN);
					break;
				case QLocale::Egypt:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_EGYPT);
					break;
				case QLocale::Iraq:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_IRAQ);
					break;
				case QLocale::Jordan:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_JORDAN);
					break;
				case QLocale::Kuwait:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_KUWAIT);
					break;
				case QLocale::Lebanon:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_LEBANON);
					break;
#				if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
				case QLocale::LibyanArabJamahiriya:
#				else
				case QLocale::Libya:
#				endif
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_LIBYA);
					break;
				case QLocale::Morocco:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_MOROCCO);
					break;
				case QLocale::Oman:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_OMAN);
					break;
				case QLocale::Qatar:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_QATAR);
					break;
				case QLocale::SaudiArabia:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_SAUDI_ARABIA);
					break;
				case QLocale::SyrianArabRepublic:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_SYRIA);
					break;
				case QLocale::Tunisia:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_TUNISIA);
					break;
				case QLocale::UnitedArabEmirates:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_UAE);
					break;
				case QLocale::Yemen:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_YEMEN);
					break;
				default:
					langId = MAKELANGID(LANG_ARABIC, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Armenian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Armenia);
			langId = MAKELANGID(LANG_ARMENIAN, SUBLANG_ARMENIAN_ARMENIA);
			break;
		case QLocale::Assamese:
			CPPDEVTK_ASSERT(locale.country() == QLocale::India);
			langId = MAKELANGID(LANG_ASSAMESE, SUBLANG_ASSAMESE_INDIA);
			break;
		case QLocale::Azerbaijani:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Azerbaijan);
			langId = MAKELANGID(LANG_AZERI, SUBLANG_AZERI_CYRILLIC);
			break;
		case QLocale::Bashkir:
			CPPDEVTK_ASSERT(locale.country() == QLocale::RussianFederation);
			langId = MAKELANGID(LANG_BASHKIR, SUBLANG_BASHKIR_RUSSIA);
			break;
		case QLocale::Basque:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Spain);
			langId = MAKELANGID(LANG_BASQUE, SUBLANG_BASQUE_BASQUE);
			break;
		case QLocale::Bosnian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::BosniaAndHerzegowina);
			langId = MAKELANGID(LANG_BOSNIAN, SUBLANG_BOSNIAN_BOSNIA_HERZEGOVINA_CYRILLIC);
			break;
		case QLocale::Breton:
			CPPDEVTK_ASSERT(locale.country() == QLocale::France);
			langId = MAKELANGID(LANG_BRETON, SUBLANG_BRETON_FRANCE);
			break;
		case QLocale::Bulgarian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Bulgaria);
			langId = MAKELANGID(LANG_BULGARIAN, SUBLANG_BULGARIAN_BULGARIA);
			break;
		/*
		case QLocale::Kurdish:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Iraq);
			langId = MAKELANGID(LANG_CENTRAL_KURDISH, SUBLANG_CENTRAL_KURDISH_IRAQ);
			break;
		*/
		/*
		case QLocale::Cherokee:
			CPPDEVTK_ASSERT(locale.country() == QLocale::UnitedStates);
			langId = MAKELANGID(LANG_CHEROKEE, SUBLANG_CHEROKEE_CHEROKEE);
			break;
		*/
		case QLocale::Catalan:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Spain);
			langId = MAKELANGID(LANG_CATALAN, SUBLANG_CATALAN_CATALAN);
			break;
		case QLocale::Chinese:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_CHINESE, SUBLANG_NEUTRAL);
					break;
				case QLocale::China:
					langId = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
					break;
				case QLocale::Taiwan:
					langId = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL);
					break;
				case QLocale::HongKong:
					langId = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_HONGKONG);
					break;
				case QLocale::Macau:
					langId = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_MACAU);
					break;
				case QLocale::Singapore:
					langId = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SINGAPORE);
					break;
				default:
					langId = MAKELANGID(LANG_CHINESE, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Corsican:
			CPPDEVTK_ASSERT(locale.country() == QLocale::France);
			langId = MAKELANGID(LANG_CORSICAN, SUBLANG_CORSICAN_FRANCE);
			break;
		case QLocale::Croatian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Croatia);
			langId = MAKELANGID(LANG_CROATIAN, SUBLANG_CROATIAN_CROATIA);
			break;
		case QLocale::Czech:
			CPPDEVTK_ASSERT(locale.country() == QLocale::CzechRepublic);
			langId = MAKELANGID(LANG_CZECH, SUBLANG_CZECH_CZECH_REPUBLIC);
			break;
		case QLocale::Danish:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Denmark);
			langId = MAKELANGID(LANG_DANISH, SUBLANG_DANISH_DENMARK);
			break;
		case QLocale::Divehi:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Maldives);
			langId = MAKELANGID(LANG_DIVEHI, SUBLANG_DIVEHI_MALDIVES);
			break;
		case QLocale::Dutch:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_DUTCH, SUBLANG_NEUTRAL);
					break;
				case QLocale::Belgium:
					langId = MAKELANGID(LANG_DUTCH, SUBLANG_DUTCH_BELGIAN);
					break;
				case QLocale::Netherlands:
					langId = MAKELANGID(LANG_DUTCH, SUBLANG_DUTCH);
					break;
				default:
					langId = MAKELANGID(LANG_DUTCH, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::English:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_NEUTRAL);
					break;
				case QLocale::UnitedStates:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
					break;
				case QLocale::UnitedKingdom:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_UK);
					break;
				case QLocale::Australia:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_AUS);
					break;
				case QLocale::Belize:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_BELIZE);
					break;
				case QLocale::Canada:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_CAN);
					break;
				case QLocale::India:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_INDIA);
					break;
				case QLocale::Ireland:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_EIRE);
					break;
				case QLocale::Jamaica:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_JAMAICA);
					break;
				case QLocale::Malaysia:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_MALAYSIA);
					break;
				case QLocale::NewZealand:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_NZ);
					break;
				case QLocale::Philippines:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_PHILIPPINES);
					break;
				case QLocale::Singapore:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_SINGAPORE);
					break;
				case QLocale::SouthAfrica:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_SOUTH_AFRICA);
					break;
				case QLocale::TrinidadAndTobago:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_TRINIDAD);
					break;
				case QLocale::Zimbabwe:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_ZIMBABWE);
					break;
				default:
					langId = MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Estonian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Estonia);
			langId = MAKELANGID(LANG_ESTONIAN, SUBLANG_ESTONIAN_ESTONIA);
			break;
		case QLocale::Faroese:
			CPPDEVTK_ASSERT(locale.country() == QLocale::FaroeIslands);
			langId = MAKELANGID(LANG_FAEROESE, SUBLANG_FAEROESE_FAROE_ISLANDS);
			break;
		case QLocale::Filipino:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Philippines);
			langId = MAKELANGID(LANG_FILIPINO, SUBLANG_FILIPINO_PHILIPPINES);
			break;
		case QLocale::Finnish:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Finland);
			langId = MAKELANGID(LANG_FINNISH, SUBLANG_FINNISH_FINLAND);
			break;
		case QLocale::French:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_FRENCH, SUBLANG_NEUTRAL);
					break;
				case QLocale::Belgium:
					langId = MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH_BELGIAN);
					break;
				case QLocale::Canada:
					langId = MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH_CANADIAN);
					break;
				case QLocale::France:
					langId = MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH);
					break;
				case QLocale::Luxembourg:
					langId = MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH_LUXEMBOURG);
					break;
				case QLocale::Monaco:
					langId = MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH_MONACO);
					break;
				case QLocale::Switzerland:
					langId = MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH_SWISS);
					break;
				default:
					langId = MAKELANGID(LANG_FRENCH, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Frisian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Netherlands);
			langId = MAKELANGID(LANG_FRISIAN, SUBLANG_FRISIAN_NETHERLANDS);
			break;
		case QLocale::Galician:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Spain);
			langId = MAKELANGID(LANG_GALICIAN, SUBLANG_GALICIAN_GALICIAN);
			break;
		case QLocale::Georgian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Georgia);
			langId = MAKELANGID(LANG_GEORGIAN, SUBLANG_GEORGIAN_GEORGIA);
			break;
		case QLocale::German:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_GERMAN, SUBLANG_NEUTRAL);
					break;
				case QLocale::Germany:
					langId = MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN);
					break;
				case QLocale::Austria:
					langId = MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN_AUSTRIAN);
					break;
				case QLocale::Liechtenstein:
					langId = MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN_LIECHTENSTEIN);
					break;
				case QLocale::Luxembourg:
					langId = MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN_LUXEMBOURG);
					break;
				case QLocale::Switzerland:
					langId = MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN_SWISS);
					break;
				default:
					langId = MAKELANGID(LANG_GERMAN, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Greek:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Greece);
			langId = MAKELANGID(LANG_GREEK, SUBLANG_GREEK_GREECE);
			break;
		case QLocale::Greenlandic:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Greenland);
			langId = MAKELANGID(LANG_GREENLANDIC, SUBLANG_GREENLANDIC_GREENLAND);
			break;
		case QLocale::Gujarati:
			CPPDEVTK_ASSERT(locale.country() == QLocale::India);
			langId = MAKELANGID(LANG_GUJARATI, SUBLANG_GUJARATI_INDIA);
			break;
		/*
		case QLocale::Hausa:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Nigeria);
			langId = MAKELANGID(LANG_HAUSA, SUBLANG_HAUSA_NIGERIA_LATIN);
			break;
		*/
		/*
		case QLocale::Hawaiian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::UnitedStates);
			langId = MAKELANGID(LANG_HAWAIIAN, SUBLANG_HAWAIIAN_US);
			break;
		*/
		case QLocale::Hebrew:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Israel);
			langId = MAKELANGID(LANG_HEBREW, SUBLANG_HEBREW_ISRAEL);
			break;
		case QLocale::Hindi:
			CPPDEVTK_ASSERT(locale.country() == QLocale::India);
			langId = MAKELANGID(LANG_HINDI, SUBLANG_HINDI_INDIA);
			break;
		case QLocale::Hungarian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Hungary);
			langId = MAKELANGID(LANG_HUNGARIAN, SUBLANG_HUNGARIAN_HUNGARY);
			break;
		case QLocale::Icelandic:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Iceland);
			langId = MAKELANGID(LANG_ICELANDIC, SUBLANG_ICELANDIC_ICELAND);
			break;
		case QLocale::Igbo:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Nigeria);
			langId = MAKELANGID(LANG_IGBO, SUBLANG_IGBO_NIGERIA);
			break;
		case QLocale::Indonesian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Indonesia);
			langId = MAKELANGID(LANG_INDONESIAN, SUBLANG_INDONESIAN_INDONESIA);
			break;
		case QLocale::Inuktitut:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Canada);
			langId = MAKELANGID(LANG_INUKTITUT, SUBLANG_INUKTITUT_CANADA);
			break;
		case QLocale::Irish:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Ireland);
			langId = MAKELANGID(LANG_IRISH, SUBLANG_IRISH_IRELAND);
			break;
		case QLocale::Xhosa:
			CPPDEVTK_ASSERT(locale.country() == QLocale::SouthAfrica);
			langId = MAKELANGID(LANG_XHOSA, SUBLANG_XHOSA_SOUTH_AFRICA);
			break;
		case QLocale::Zulu:
			CPPDEVTK_ASSERT(locale.country() == QLocale::SouthAfrica);
			langId = MAKELANGID(LANG_ZULU, SUBLANG_ZULU_SOUTH_AFRICA);
			break;
		case QLocale::Italian:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_ITALIAN, SUBLANG_NEUTRAL);
					break;
				case QLocale::Italy:
					langId = MAKELANGID(LANG_ITALIAN, SUBLANG_ITALIAN);
					break;
				case QLocale::Switzerland:
					langId = MAKELANGID(LANG_ITALIAN, SUBLANG_ITALIAN_SWISS);
					break;
				default:
					langId = MAKELANGID(LANG_ITALIAN, SUBLANG_DEFAULT);
					break;
			}
		case QLocale::Japanese:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Japan);
			langId = MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN);
			break;
		case QLocale::Kannada:
			CPPDEVTK_ASSERT(locale.country() == QLocale::India);
			langId = MAKELANGID(LANG_KANNADA, SUBLANG_KANNADA_INDIA);
			break;
		case QLocale::Kashmiri:
			CPPDEVTK_ASSERT(locale.country() == QLocale::India);
			langId = MAKELANGID(LANG_KASHMIRI, SUBLANG_KASHMIRI_INDIA);
			break;
		case QLocale::Kazakh:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Kazakhstan);
			langId = MAKELANGID(LANG_KAZAK, SUBLANG_KAZAK_KAZAKHSTAN);
			break;
		case QLocale::Cambodian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Cambodia);
			langId = MAKELANGID(LANG_KHMER, SUBLANG_KHMER_CAMBODIA);
			break;
		case QLocale::Kinyarwanda:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Rwanda);
			langId = MAKELANGID(LANG_KINYARWANDA, SUBLANG_KINYARWANDA_RWANDA);
			break;
		case QLocale::Konkani:
			CPPDEVTK_ASSERT(locale.country() == QLocale::India);
			langId = MAKELANGID(LANG_KONKANI, SUBLANG_KONKANI_INDIA);
			break;
		case QLocale::Korean:
			CPPDEVTK_ASSERT((locale.country() == QLocale::RepublicOfKorea) || (locale.country() == QLocale::DemocraticRepublicOfKorea));
			langId = MAKELANGID(LANG_KOREAN, SUBLANG_KOREAN);
			break;
#		if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
		case QLocale::Laothian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Lao);
			langId = MAKELANGID(LANG_LAO, SUBLANG_LAO_LAO);
			break;
#		endif
		case QLocale::Latvian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Latvia);
			langId = MAKELANGID(LANG_LATVIAN, SUBLANG_LATVIAN_LATVIA);
			break;
		/*
		case QLocale::Lithuanian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Lithuania);
			langId = MAKELANGID(LANG_LITHUANIAN, SUBLANG_LITHUANIAN_LITHUANIA);
			break;
		*/
		case QLocale::LowGerman:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Germany);
			langId = MAKELANGID(LANG_LOWER_SORBIAN, SUBLANG_LOWER_SORBIAN_GERMANY);
			break;
		case QLocale::Macedonian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Macedonia);
			langId = MAKELANGID(LANG_MACEDONIAN, SUBLANG_MACEDONIAN_MACEDONIA);
			break;
		case QLocale::Malay:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_MALAY, SUBLANG_NEUTRAL);
					break;
#				if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
				case QLocale::BruneiDarussalam:
#				else
				case QLocale::Brunei:
#				endif
					langId = MAKELANGID(LANG_MALAY, SUBLANG_MALAY_BRUNEI_DARUSSALAM);
					break;
				case QLocale::Malaysia:
					langId = MAKELANGID(LANG_MALAY, SUBLANG_MALAY_MALAYSIA);
					break;
				default:
					langId = MAKELANGID(LANG_MALAY, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Malayalam:
			CPPDEVTK_ASSERT(locale.country() == QLocale::India);
			langId = MAKELANGID(LANG_MALAYALAM, SUBLANG_MALAYALAM_INDIA);
			break;
		case QLocale::Maltese:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Malta);
			langId = MAKELANGID(LANG_MALTESE, SUBLANG_MALTESE_MALTA);
			break;
		case QLocale::Maori:
			CPPDEVTK_ASSERT(locale.country() == QLocale::NewZealand);
			langId = MAKELANGID(LANG_MAORI, SUBLANG_MAORI_NEW_ZEALAND);
			break;
		case QLocale::Marathi:
			CPPDEVTK_ASSERT(locale.country() == QLocale::India);
			langId = MAKELANGID(LANG_MARATHI, SUBLANG_MARATHI_INDIA);
			break;
		case QLocale::Mongolian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Mongolia);
			langId = MAKELANGID(LANG_MONGOLIAN, SUBLANG_MONGOLIAN_PRC);
			break;
		case QLocale::Nepali:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Nepal);
			langId = MAKELANGID(LANG_NEPALI, SUBLANG_NEPALI_NEPAL);
			break;
		case QLocale::Norwegian:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_NORWEGIAN, SUBLANG_NEUTRAL);
					break;
				case QLocale::NorwegianBokmal:
					langId = MAKELANGID(LANG_NORWEGIAN, SUBLANG_NORWEGIAN_BOKMAL);
					break;
				case QLocale::NorwegianNynorsk:
					langId = MAKELANGID(LANG_NORWEGIAN, SUBLANG_NORWEGIAN_NYNORSK);
					break;
				default:
					langId = MAKELANGID(LANG_NORWEGIAN, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Occitan:
			CPPDEVTK_ASSERT(locale.country() == QLocale::France);
			langId = MAKELANGID(LANG_OCCITAN, SUBLANG_OCCITAN_FRANCE);
			break;
		case QLocale::Oriya:
			CPPDEVTK_ASSERT(locale.country() == QLocale::India);
			langId = MAKELANGID(LANG_ORIYA, SUBLANG_ORIYA_INDIA);
			break;
		case QLocale::Pashto:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Afghanistan);
			langId = MAKELANGID(LANG_PASHTO, SUBLANG_PASHTO_AFGHANISTAN);
			break;
		case QLocale::Persian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Iran);
			langId = MAKELANGID(LANG_PERSIAN, SUBLANG_PERSIAN_IRAN);
			break;
		case QLocale::Polish:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Poland);
			langId = MAKELANGID(LANG_POLISH, SUBLANG_POLISH_POLAND);
			break;
		case QLocale::Portuguese:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_PORTUGUESE, SUBLANG_NEUTRAL);
					break;
				case QLocale::Brazil:
					langId = MAKELANGID(LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN);
					break;
				case QLocale::Portugal:
					langId = MAKELANGID(LANG_PORTUGUESE, SUBLANG_PORTUGUESE);
					break;
				default:
					langId = MAKELANGID(LANG_PORTUGUESE, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Punjabi:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_PUNJABI, SUBLANG_NEUTRAL);
					break;
				case QLocale::India:
					langId = MAKELANGID(LANG_PUNJABI, SUBLANG_PUNJABI_INDIA);
					break;
				/*
				case QLocale::Pakistan:
					langId = MAKELANGID(LANG_PUNJABI, SUBLANG_PUNJABI_PAKISTAN);
					break;
				*/
				default:
					langId = MAKELANGID(LANG_PUNJABI, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Quechua:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_QUECHUA, SUBLANG_NEUTRAL);
					break;
				case QLocale::Bolivia:
					langId = MAKELANGID(LANG_QUECHUA, SUBLANG_QUECHUA_BOLIVIA);
					break;
				case QLocale::Ecuador:
					langId = MAKELANGID(LANG_QUECHUA, SUBLANG_QUECHUA_ECUADOR);
					break;
				case QLocale::Peru:
					langId = MAKELANGID(LANG_QUECHUA, SUBLANG_QUECHUA_PERU);
					break;
				default:
					langId = MAKELANGID(LANG_QUECHUA, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Romanian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Romania);
			langId = MAKELANGID(LANG_ROMANIAN, SUBLANG_ROMANIAN_ROMANIA);
			break;
		case QLocale::Russian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::RussianFederation);
			langId = MAKELANGID(LANG_RUSSIAN, SUBLANG_RUSSIAN_RUSSIA);
			break;
		case QLocale::Sanskrit:
			CPPDEVTK_ASSERT(locale.country() == QLocale::India);
			langId = MAKELANGID(LANG_SANSKRIT, SUBLANG_SANSKRIT_INDIA);
			break;
		case QLocale::Serbian:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_SERBIAN, SUBLANG_NEUTRAL);
					break;
				case QLocale::BosniaAndHerzegowina:
					langId = MAKELANGID(LANG_SERBIAN, SUBLANG_SERBIAN_BOSNIA_HERZEGOVINA_CYRILLIC);
					break;
				case QLocale::Croatia:
					langId = MAKELANGID(LANG_SERBIAN, SUBLANG_SERBIAN_CROATIA);
					break;
#				if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
				case QLocale::SerbiaAndMontenegro:
#				else
				case QLocale::Serbia:
#				endif
					langId = MAKELANGID(LANG_SERBIAN, SUBLANG_SERBIAN_CYRILLIC);
					break;
				default:
					langId = MAKELANGID(LANG_SERBIAN, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Sindhi:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_SINDHI, SUBLANG_NEUTRAL);
					break;
				case QLocale::Afghanistan:
					langId = MAKELANGID(LANG_SINDHI, SUBLANG_SINDHI_AFGHANISTAN);
					break;
				case QLocale::India:
					langId = MAKELANGID(LANG_SINDHI, SUBLANG_SINDHI_INDIA);
					break;
				case QLocale::Pakistan:
					langId = MAKELANGID(LANG_SINDHI, SUBLANG_SINDHI_PAKISTAN);
					break;
				default:
					langId = MAKELANGID(LANG_SINDHI, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Slovak:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Slovakia);
			langId = MAKELANGID(LANG_SLOVAK, SUBLANG_SLOVAK_SLOVAKIA);
			break;
		case QLocale::Slovenian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Slovenia);
			langId = MAKELANGID(LANG_SLOVENIAN, SUBLANG_SLOVENIAN_SLOVENIA);
			break;
		case QLocale::Spanish:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_NEUTRAL);
					break;
				case QLocale::Argentina:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_ARGENTINA);
					break;
				case QLocale::Bolivia:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_BOLIVIA);
					break;
				case QLocale::Chile:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_CHILE);
					break;
				case QLocale::Colombia:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_COLOMBIA);
					break;
				case QLocale::CostaRica:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_COSTA_RICA);
					break;
				case QLocale::DominicanRepublic:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_DOMINICAN_REPUBLIC);
					break;
				case QLocale::Ecuador:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_ECUADOR);
					break;
				case QLocale::ElSalvador:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_EL_SALVADOR);
					break;
				case QLocale::Guatemala:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_GUATEMALA);
					break;
				case QLocale::Honduras:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_HONDURAS);
					break;
				case QLocale::Mexico:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_MEXICAN);
					break;
				case QLocale::Nicaragua:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_NICARAGUA);
					break;
				case QLocale::Panama:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_PANAMA);
					break;
				case QLocale::Paraguay:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_PARAGUAY);
					break;
				case QLocale::Peru:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_PERU);
					break;
				case QLocale::PuertoRico:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_PUERTO_RICO);
					break;
				case QLocale::Spain:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH);
					break;
				case QLocale::UnitedStates:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_US);
					break;
				case QLocale::Uruguay:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_URUGUAY);
					break;
				case QLocale::Venezuela:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_VENEZUELA);
					break;
				default:
					langId = MAKELANGID(LANG_SPANISH, SUBLANG_DEFAULT);
					break;
			}
			break;
		/*
		case QLocale::Swahili:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Kenya);
			langId = MAKELANGID(LANG_SWAHILI, SUBLANG_SWAHILI);
			break;
		*/
		case QLocale::Swedish:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_SWEDISH, SUBLANG_NEUTRAL);
					break;
				case QLocale::Finland:
					langId = MAKELANGID(LANG_SWEDISH, SUBLANG_SWEDISH_FINLAND);
					break;
				case QLocale::Sweden:
					langId = MAKELANGID(LANG_SWEDISH, SUBLANG_SWEDISH);
					break;
				default:
					langId = MAKELANGID(LANG_SWEDISH, SUBLANG_DEFAULT);
					break;
			}
			break;
		/*
		case QLocale::Syriac:
			CPPDEVTK_ASSERT(locale.country() == QLocale::SyrianArabRepublic);
			langId = MAKELANGID(LANG_SYRIAC, SUBLANG_SYRIAC);
			break;
		*/
		case QLocale::Tajik:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Tajikistan);
			langId = MAKELANGID(LANG_TAJIK, SUBLANG_TAJIK_TAJIKISTAN);
			break;
		case QLocale::Tamil:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_TAMIL, SUBLANG_NEUTRAL);
					break;
				case QLocale::India:
					langId = MAKELANGID(LANG_TAMIL, SUBLANG_TAMIL_INDIA);
					break;
				/*
				case QLocale::SriLanka:
					langId = MAKELANGID(LANG_TAMIL, SUBLANG_TAMIL_SRI_LANKA);
					break;
				*/
				default:
					langId = MAKELANGID(LANG_TAMIL, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Tatar:
			CPPDEVTK_ASSERT(locale.country() == QLocale::RussianFederation);
			langId = MAKELANGID(LANG_TATAR, SUBLANG_TATAR_RUSSIA);
			break;
		case QLocale::Telugu:
			CPPDEVTK_ASSERT(locale.country() == QLocale::India);
			langId = MAKELANGID(LANG_TELUGU, SUBLANG_TELUGU_INDIA);
			break;
		case QLocale::Thai:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Thailand);
			langId = MAKELANGID(LANG_THAI, SUBLANG_THAI_THAILAND);
			break;
		case QLocale::Tibetan:
			CPPDEVTK_ASSERT(locale.country() == QLocale::China);
			langId = MAKELANGID(LANG_TIBETAN, SUBLANG_TIBETAN_PRC);
			break;
		/*
		case QLocale::Tigrinya:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_TIGRINYA, SUBLANG_NEUTRAL);
					break;
				case QLocale::Eritrea:
					langId = MAKELANGID(LANG_TIGRINYA, SUBLANG_TIGRINYA_ERITREA);
					break;
				case QLocale::Ethiopia:
					langId = MAKELANGID(LANG_TIGRINYA, SUBLANG_TIGRINYA_ETHIOPIA);
					break;
				default:
					langId = MAKELANGID(LANG_TIGRINYA, SUBLANG_DEFAULT);
					break;
			}
			break;
		*/
		case QLocale::Turkish:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Turkey);
			langId = MAKELANGID(LANG_TURKISH, SUBLANG_TURKISH_TURKEY);
			break;
		case QLocale::Turkmen:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Turkmenistan);
			langId = MAKELANGID(LANG_TURKMEN, SUBLANG_TURKMEN_TURKMENISTAN);
			break;
		case QLocale::Ukrainian:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Ukraine);
			langId = MAKELANGID(LANG_UKRAINIAN, SUBLANG_UKRAINIAN_UKRAINE);
			break;
		case QLocale::Urdu:
			switch (locale.country()) {
				case QLocale::AnyCountry:
					langId = MAKELANGID(LANG_URDU, SUBLANG_NEUTRAL);
					break;
				case QLocale::India:
					langId = MAKELANGID(LANG_URDU, SUBLANG_URDU_INDIA);
					break;
				case QLocale::Pakistan:
					langId = MAKELANGID(LANG_URDU, SUBLANG_URDU_PAKISTAN);
					break;
				default:
					langId = MAKELANGID(LANG_URDU, SUBLANG_DEFAULT);
					break;
			}
			break;
		case QLocale::Uigur:
			CPPDEVTK_ASSERT(locale.country() == QLocale::China);
			langId = MAKELANGID(LANG_UIGHUR, SUBLANG_UIGHUR_PRC);
			break;
		case QLocale::Uzbek:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Uzbekistan);
			langId = MAKELANGID(LANG_UZBEK, SUBLANG_UZBEK_CYRILLIC);
			break;
		case QLocale::Vietnamese:
#			if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
			CPPDEVTK_ASSERT(locale.country() == QLocale::VietNam);
#			else
			CPPDEVTK_ASSERT(locale.country() == QLocale::Vietnam);
#			endif
			langId = MAKELANGID(LANG_VIETNAMESE, SUBLANG_VIETNAMESE_VIETNAM);
			break;
		case QLocale::Welsh:
			CPPDEVTK_ASSERT(locale.country() == QLocale::UnitedKingdom);
			langId = MAKELANGID(LANG_WELSH, SUBLANG_WELSH_UNITED_KINGDOM);
			break;
		case QLocale::Wolof:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Senegal);
			langId = MAKELANGID(LANG_WOLOF, SUBLANG_WOLOF_SENEGAL);
			break;
		case QLocale::Yoruba:
			CPPDEVTK_ASSERT(locale.country() == QLocale::Nigeria);
			langId = MAKELANGID(LANG_YORUBA, SUBLANG_YORUBA_NIGERIA);
			break;
		default:
			CPPDEVTK_LOG_WARN("unsupported locale.language(): " << locale.language());
			//CPPDEVTK_ASSERT(0 && "unsupported locale.language()");
			
			//langId = MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT);	// LANG_SYSTEM_DEFAULT
			langId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);	// LANG_USER_DEFAULT
			//langId = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);	// Language neutral
			break;
	}
	
	//CPPDEVTK_LOG_TRACE("locale.language(): " << locale.language() << "; langId: " << NumToHexStr(langId));
	
	return langId;
}


}	// namespace base
}	// namespace cppdevtk
