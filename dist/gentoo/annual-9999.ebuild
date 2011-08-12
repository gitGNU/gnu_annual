# Copyright 1999-2009 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $
SNAPSHOT="yes"
EAPI="2"

inherit git cmake-utils

DESCRIPTION="Keeps track of your anniversaries and reminds you hopefully at time."
HOMEPAGE="http://annual.berlios.de/"
EGIT_REPO_URI="git://git.berlios.de/annual"
SRC_URI=""
LANGUAGES="linguas_en linguas_de"

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="~x86 ~amd64"

DEPEND="
	dev-vcs/git
	>=x11-libs/qt-gui-4.4.2
	"
RDEPEND="
	dev-lang/perl
	dev-perl/XML-XPath
	dev-perl/Log-Log4perl
	>=x11-libs/qt-gui-4.4.2
"
RESTRICT=""

src_unpack() {
	git_src_unpack
}
