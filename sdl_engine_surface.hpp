Surface::Surface()
{
    pimpl=new impl;
}
Surface::~Surface()
{
    delete pimpl;
}
Surface::Surface(const Surface& inc) : Surface()
{
    *pimpl=*(inc.pimpl);
}
